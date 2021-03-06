#include "state.hpp"
#include "../server.hpp"

void ares::account::character_server::packet_handler<ares::packet::current<ares::packet::ATHENA_HA_SET_AID_OFFLINE>>::operator()() {
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_SET_AID_OFFLINE: begin");
  std::lock_guard<std::mutex> lock(server_.mutex());
  server_.unlink_aid_from_char_server(p_->aid(), session_.shared_from_this());
  SPDLOG_TRACE(log(), "handle_packet ATHENA_HA_SET_AID_OFFLINE: end");
}
