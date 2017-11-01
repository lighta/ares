#pragma once

namespace ares {
  template <>
  struct packet_ids<packet_version::base> {
    static const uint16_t AC_REFUSE_LOGIN = 0x6a;
    static const uint16_t SC_NOTIFY_BAN = 0x81;
    static const uint16_t CA_EXE_HASHCHECK = 0x204;
    static const uint16_t CA_SSO_LOGIN_REQ = 0x825;
    static const uint16_t AC_ACCEPT_LOGIN = 0xac4;
    static const uint16_t CH_ENTER = 0x65;
    static const uint16_t HC_REFUSE_ENTER = 0x6c;

    // Athena packets
    static const uint16_t ATHENA_HA_LOGIN_REQ = 0x2710;
    static const uint16_t ATHENA_AH_LOGIN_RESULT = 0x2711;
    static const uint16_t ATHENA_HA_PING_REQ = 0x2719;
    static const uint16_t ATHENA_AH_PING_ACK = 0x2718;
    static const uint16_t ATHENA_HA_AID_AUTH_REQ = 0x2712;
    static const uint16_t ATHENA_AH_AID_AUTH_RESULT = 0x2713;
    static const uint16_t ATHENA_HA_USER_COUNT = 0x2714;
    static const uint16_t ATHENA_HA_ACCOUNT_DATA_REQ = 0x2716;
    static const uint16_t ATHENA_AH_ACCOUNT_DATA = 0x2717;
    static const uint16_t ATHENA_HA_SET_AID_ONLINE = 0x272b;
    static const uint16_t ATHENA_HA_SET_AID_OFFLINE = 0x272c;
    static const uint16_t ATHENA_HA_ONLINE_AIDS = 0x272d;
    static const uint16_t ATHENA_HA_SET_ALL_AIDS_OFFLINE = 0x2737;

    static const uint16_t ATHENA_ZH_LOGIN_REQ = 0x2af8;
    static const uint16_t ATHENA_HZ_LOGIN_RESULT = 0x2af9;
    static const uint16_t ATHENA_ZH_MAP_NAMES = 0x2afa;
    static const uint16_t ATHENA_HZ_PRIVATE_MSG_NAME = 0x2afb;
    static const uint16_t ATHENA_ZH_PING_REQ = 0x2b23;
    static const uint16_t ATHENA_HZ_PING_ACK = 0x2b24;
    static const uint16_t ATHENA_ZH_ONLINE_USERS = 0x2aff;
    static const uint16_t ATHENA_ZH_USER_COUNT = 0x2afe;
    static const uint16_t ATHENA_ZH_GAME_RATES = 0x2b16;
  };
}