#pragma once

#include <iostream>

#include <ares/packets>

#include "../session.hpp"

template <typename Derived, typename Server>
ares::network::session<Derived, Server>::session(Server& server,
                                                 const std::optional<asio::ip::tcp::endpoint>& connect_ep,
                                                 std::shared_ptr<asio::ip::tcp::socket> socket,
                                                 const std::chrono::seconds idle_timer_timeout) :
  recv_buf_busy_(false),
  send_buf_busy_(false),
  connected_(true),
  socket_(socket),
  server_(server),
  connecting_(false),
  receiving_(false),
  sending_(false),
  connect_ep_(connect_ep),
  idle_timer_(std::make_shared<asio::steady_timer>(*server.io_context())),
  idle_timer_timeout_(idle_timer_timeout) {
  if (socket_) {
    // TODO: Make configurable
    socket_->set_option(asio::ip::tcp::no_delay(true));
  } else {
    connected_ = false;
  }
  }

template <typename Derived, typename Server>
void ares::network::session<Derived, Server>::defuse() {
  if (idle_timer_) idle_timer_->cancel();
  if (reconnect_timer_) reconnect_timer_->cancel();
}

template <typename Derived, typename Server>
inline void ares::network::session<Derived, Server>::receive(const size_t receive_sz) {
  if (connected_ && !receiving_ && socket_) {
    receiving_ = true;
    auto sz = receive_sz;
    if (receive_sz == 0) {
      sz = recv_buf_.unfragmented_free_size();
    }
    if (recv_buf_.free_size() >= sz) {
      // TODO handler with recv_mutex
      if (recv_buf_.unfragmented_free_size() < sz) { recv_buf_.defragment(); }
      SPDLOG_TRACE(log(), "receive requesting {} bytes", sz);
      auto h = receive_handler<Derived>(static_cast<Derived&>(*this).shared_from_this());
      this->socket_->async_read_some(asio::buffer(recv_buf_.end(), sz), h);
    } else {
      log()->error("receive: out of buffer space, session {0:#x}", (uintptr_t)this);
      server_.close_gracefuly(static_cast<Derived&>(*this).shared_from_this());
    }
  }
}

template <typename Derived, typename Server>
template <typename Packet>
inline void ares::network::session<Derived, Server>::copy_and_send(const Packet& p) {
  SPDLOG_TRACE(log(), "network::session::send(Packet) acquiring send lock");
  std::unique_lock<std::mutex> lock(send_mutex_);
  if (connected_ && socket_) {
    if (packet::size::validate<Packet>(p)) {
      const auto send_sz = packet::size::get<Packet>(&p, sizeof(Packet));
      SPDLOG_TRACE(log(), "network::session::send(Packet) queueing {} bytes to send", send_sz); 
      if (send_sz > 0) {
        lock.unlock();
        copy_and_send(&p, send_sz);
      } else {
        lock.unlock();
        log()->error("Packet send failed, can't determine size to send");
      }
    } else {
      lock.unlock();
      log()->error("Packet send failed, size validation failed, sizeof(Packet) = {}", sizeof(Packet));
    }
  } else {
    lock.unlock();
    log()->error("Packet send failed, attempted to send over unconnected socket");
  }
}

template <typename Derived, typename Server>
inline void ares::network::session<Derived, Server>::send_wake_up(const void* data_start) {
  if (!sending_) {
    sending_ = true;
    SPDLOG_TRACE(log(), "Send handler wakeup, {} bytes to send", send_buf_.size());
    auto h = send_handler<Derived>(static_cast<Derived&>(*this).shared_from_this());
    socket_->async_write_some(asio::buffer(data_start, send_buf_.head_size()), std::move(h));
  } else {
    SPDLOG_TRACE(log(), "Send handler wakeup ignoring, {} bytes to send, but a send operation is pending", send_buf_.size());
  }
}

template <typename Derived, typename Server>
inline void ares::network::session<Derived, Server>::copy_and_send(const void* data, const size_t send_sz) {
  SPDLOG_TRACE(log(), "network::session::send acquiring send lock");
  std::lock_guard<std::mutex> lock(send_mutex_);
  if (connected_ && socket_) {
    if (send_buf_.free_size() >= send_sz) {
      const auto p = send_buf_.end();
      SPDLOG_TRACE(log(), "sending {} bytes, copying to {:#x}, offset from buf start {}, already in buf {}",
                   send_sz, (uintptr_t)send_buf_.end(), (uintptr_t)send_buf_.end() - (uintptr_t)send_buf_.begin(), send_buf_.size());
      send_buf_.push_back(data, send_sz);
      send_wake_up(p);
    } else {
      // TODO: autogrow, but make sure asio/rxcpp is not using the buffer
      log()->error("send: out of buffer space, session {0:#x}", (uintptr_t)this);
      auto s = static_cast<Derived&>(*this).shared_from_this();
      server_.close_gracefuly(s);
    }
  }
}

template <typename Derived, typename Server>
template <typename Packet, typename... Args>
inline void ares::network::session<Derived, Server>::emplace_and_send(Args&&... args) {
  SPDLOG_TRACE(log(), "emplace_and_send acquiring send lock");
  std::lock_guard<std::mutex> lock(send_mutex_);
  if (connected_ && socket_) {
    SPDLOG_TRACE(log(), "emplace_and_send lock acquired");
    const auto header_sz = sizeof(Packet);
    if (header_sz <= send_buf_.free_size()) {
      if (header_sz > send_buf_.unfragmented_free_size()) {
        if (sending_) {
          // Fallback to copy-send, because we can't defragment buffer space while asio is busy
          Packet pck(std::forward<Args>(args)...);
          copy_and_send<Packet>(pck);
          return;
        } else {
          send_buf_.defragment();
        }
      }
      auto p = (Packet*)send_buf_.end();
      p->emplace(std::forward<Args>(args)...);
      send_buf_.adjust_size_by(header_sz);
      send_wake_up(p);
    } else {
      // TODO: autogrow, but make sure asio is not using the buffer
      log()->error("send: out of buffer space, session {0:#x}", (uintptr_t)this);
      auto s = static_cast<Derived&>(*this).shared_from_this();
      server_.close_gracefuly(s);
    }
  } else {
    log()->error("Packet send failed, attempted to send over unconnected socket");
  }
}

template <typename Derived, typename Server>
inline auto ares::network::session<Derived, Server>::io_context() const -> std::shared_ptr<asio::io_context> {
  return server_.io_context();
}

template <typename Derived, typename Server>
inline auto ares::network::session<Derived, Server>::log() const -> std::shared_ptr<spdlog::logger> {
  return server_.log();
}

template <typename Derived, typename Server>
inline auto ares::network::session<Derived, Server>::server() -> Server& {
  return server_;
}

template <typename Derived, typename Server>
inline void ares::network::session<Derived, Server>::reset_idle_timer() {
  idle_timer_->cancel();
  idle_timer_->expires_at(std::chrono::steady_clock::now() + idle_timer_timeout_);
  SPDLOG_TRACE(log(), "Idle timer creating session copy");
  auto s = static_cast<Derived*>(this)->shared_from_this();
  SPDLOG_TRACE(log(), "Idle timer created session copy");
  idle_timer_->async_wait([s] (const std::error_code& ec) {
      if (ec.value() == 0) {
        SPDLOG_TRACE(s->log(), "Idle timer for session {} fired", (void*)s.get());
        s->server().close_gracefuly(s);
      }
    });
}

template <typename Derived, typename Server>
inline void ares::network::session<Derived, Server>::set_reconnect_timer(const std::chrono::seconds first_timeout,
                                                                         const std::chrono::seconds timeout) {
  SPDLOG_TRACE(log(), "Setting reconnect timer");
  if (connect_ep_) {
    if (reconnect_timer_) {
      reconnect_timer_->cancel();
    } else {
      reconnect_timer_ = std::make_shared<asio::steady_timer>(*io_context());
    }
    reconnect_timer_->expires_at(std::chrono::steady_clock::now() + first_timeout);
    reconnect_timer_timeout_ = timeout;
    reconnect_timer_->async_wait(reconnect_timer_handler<Derived>(static_cast<Derived&>(*this).shared_from_this(), timeout));
  } else {
    SPDLOG_TRACE(log(), "Can't set reconnect timer: endpoint is none");
  }
}
