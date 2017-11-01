#pragma once

#include <map>
#include <set>

#include <ares/network>

#include "predeclare.hpp"

#include "session.hpp"
#include "database/database.hpp"

namespace ares {
  namespace character {
    struct server : ares::network::server<server> {
      server(std::shared_ptr<spdlog::logger> log,
             std::shared_ptr<boost::asio::io_service> io_service,
             const config& conf,
             const size_t num_threads);

      void start();
      void create_session(std::shared_ptr<boost::asio::ip::tcp::socket> socket);

      void add(session_ptr s);
      void remove(session_ptr s);

      session_ptr client_by_aid(const uint32_t aid);
      void link_aid_to_zone_server(const uint32_t aid, session_ptr s);
      void unlink_aid_from_zone_server(const uint32_t aid, session_ptr s);

      const config& conf() const;
      database& db();
      const std::map<uint32_t, session_ptr>& clients() const;
      const session_ptr& account_server() const;
      const std::set<session_ptr>& zone_servers() const;

      // Data
      uint16_t state_num{0};
      uint16_t property{0};

    private:
      const config& config_;
      database db_;

      std::set<session_ptr> mono_;
      std::set<session_ptr> zone_servers_;
      std::map<uint32_t, session_ptr> clients_;
      std::map<uint32_t, session_ptr> aid_to_zone_server_;
      session_ptr account_server_;

    };
  }
}