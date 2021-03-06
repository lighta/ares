#include "send_handler.hpp"

#include "session.hpp"

void ares::character::send_handler::terminate_session() {
  session_->close_socket();
  session_->remove_from_server();
}

void ares::character::send_handler::on_operation_aborted() {
  session_->on_operation_aborted();
}

void ares::character::send_handler::on_connection_reset() {
  session_->on_connection_reset();
}

void ares::character::send_handler::on_eof() {
  session_->on_eof();
}

void ares::character::send_handler::on_socket_error() {
  session_->on_socket_error();
}
