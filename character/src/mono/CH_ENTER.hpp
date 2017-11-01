template <>
struct packet_handler<packet::CH_ENTER>
  : network::handler::packet::base<packet_handler<packet::CH_ENTER>, packet::CH_ENTER,
                                   server, session, state> {

  using network::handler::packet::base<packet_handler<packet::CH_ENTER>, packet::CH_ENTER,
                                       server, session, state>::base;

  void operator()();

private:
  void from_zone_server(std::shared_ptr<session> existing_session);
  void from_account_server();

  void refuse(const uint8_t error_code);
  void notify_ban(const uint8_t error_code);
};