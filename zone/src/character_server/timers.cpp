#include "timers.hpp"

#include "state.hpp"
#include "../session.hpp"
#include "../server.hpp"

void ares::zone::character_server::timer::reconnect::on_timer() {
  if (!session_->connected()) {
    auto& server = session_->server_;
    std::lock_guard<std::mutex> lock(server.mutex());
    session_->socket() = std::make_shared<boost::asio::ip::tcp::socket>(*session_->io_service());
    boost::system::error_code ec;
    auto& ep = *server.conf().character_server->connect;
    log()->info("Connection to character server is not established. Connecting to {}:{}...",
                ep.address().to_string(), ep.port());
    session_->socket()->open(ep.protocol(), ec);

    if (ec.value() == 0) {
      auto session_copy = session_;
      session_->socket()->async_connect(ep, [session_copy] (const boost::system::error_code ec) {
          auto& server = session_copy->server_;
          std::lock_guard<std::mutex> lock(server.mutex());
          if (ec.value() == 0) {
            session_copy->set_connected();
            server.log()->info("Starting handshake with character server");
            const auto my_ipv4 = server.conf().listen_ipv4[0].address().to_v4().to_ulong();
            const auto my_port = server.conf().listen_ipv4[0].port();
             session_copy->emplace_and_send<packet::ATHENA_ZH_LOGIN_REQ>(server.conf().character_server->login,
                                                                         server.conf().character_server->password,
                                                                         0,
                                                                         htonl(my_ipv4),
                                                                         htons(my_port));
            session_copy->receive();
            session_copy->as_character_server().ping_request_timer.set();    
          } else {
            session_copy->close_socket();
            server.log()->warn("Connecting socket to character server failed. {}, {}", ec.value(), ec.message());
          }
          session_copy->as_character_server().reconnect_timer.set();
        });
    } else {
      log()->error("Opening socket to character server failed. {}, {}", ec.value(), ec.message());
    }
  } else {
    SPDLOG_TRACE(log(), "Character server is still connected, reconnect is not needed");
    session_->as_character_server().reconnect_timer.set();
  }
}

void ares::zone::character_server::timer::ping_request::on_timer() {
  auto& server = session_->server_;
  std::lock_guard<std::mutex> lock(server.mutex());
  if (server.char_server() &&
      (server.char_server() == session_) &&
      session_->connected()) {
    SPDLOG_TRACE(log(), "ping_request_handler sending ping to character server");
    session_->emplace_and_send<packet::ATHENA_HA_PING_REQ>();
    session_->as_character_server().ping_timeout_timer.set();
  } else {
    SPDLOG_TRACE(log(),
                 "ping_request_handler not sending ping to character server, not connected."
                 "server.character_server() == {}, session_ == {}, session_->connected() = {}",
                 (uintptr_t)server.char_server().get(),
                 (uintptr_t)session_.get(),
                 session_->connected());
  }
}

void ares::zone::character_server::timer::ping_timeout::on_timer() {
  auto& server = session_->server_;
  std::lock_guard<std::mutex> lock(server.mutex());
  if (server.char_server() &&
      (server.char_server() == session_) &&
      session_->connected()) {
    log()->error("Timeout while waiting for ping response from character server, closing session");
    session_->close_socket();
  }
}