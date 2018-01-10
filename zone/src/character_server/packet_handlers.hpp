#pragma once

#include <memory>
#include <optional>
#include <tuple>

#include <spdlog/spdlog.h>

#include <ares/network>
#include <ares/packets>

#include "../predeclare.hpp"

namespace ares {
  namespace zone {
    namespace character_server {
      ARES_DECLARE_PACKET_HANDLER_TEMPLATE();

      // Simple packet handlers that do not define their own class structure
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_HZ_LOGIN_RESULT);
      ARES_SIMPLE_PACKET_HANDLER(ATHENA_HZ_PRIVATE_MSG_NAME);
      // Packet handlers that store state/structured

    }
  }
}
