#pragma once

namespace ares {
  namespace packet_sets {
    /*! Client packets that have never been changed during the dev cycle.
      As soon as a packet changes, it should be moved down the packetset hierarchy.
    */

    ARES_PACKETSET_DECLARE(client_stable, server);

#define ARES_PCK_ID_NAME(ID, NAME) ARES_PACKETSET_ID_NAME(client_stable, ID, NAME)

    ARES_PCK_ID_NAME(0x81, SC_NOTIFY_BAN);
    ARES_PCK_ID_NAME(0x187, PING);

    // Account <-> Client
    ARES_PCK_ID_NAME(0x6a, AC_REFUSE_LOGIN);
    ARES_PCK_ID_NAME(0x204, CA_EXE_HASHCHECK);
    ARES_PCK_ID_NAME(0x64, CA_SSO_LOGIN_REQ::login_password);
    ARES_PCK_ID_NAME(0x825, CA_SSO_LOGIN_REQ::token_auth);
    ARES_PCK_ID_NAME(0xac4, AC_ACCEPT_LOGIN);

    // Character <-> Client
    ARES_PCK_ID_NAME(0x65, CH_ENTER);
    ARES_PCK_ID_NAME(0x66, CH_SELECT_CHAR);
    ARES_PCK_ID_NAME(0xa39, CH_MAKE_CHAR::no_stats);
    ARES_PCK_ID_NAME(0x68, CH_DELETE_CHAR);
    ARES_PCK_ID_NAME(0x71, HC_NOTIFY_ZONESVR);
    ARES_PCK_ID_NAME(0x20d, HC_BLOCK_CHARACTER);
    ARES_PCK_ID_NAME(0x82d, HC_ACCEPT_ENTER);
    ARES_PCK_ID_NAME(0x9a1, CH_CHAR_PAGE_REQ);
    ARES_PCK_ID_NAME(0x99d, HC_CHAR_PAGES);
    ARES_PCK_ID_NAME(0x6c, HC_REFUSE_ENTER);
    ARES_PCK_ID_NAME(0x6d, HC_ACCEPT_MAKECHAR);
    ARES_PCK_ID_NAME(0x6e, HC_REFUSE_MAKECHAR);

    // Zone <-> Client
    ARES_PCK_ID_NAME(0x87, ZC_NOTIFY_PLAYERMOVE);
    ARES_PCK_ID_NAME(0x2eb, ZC_ACCEPT_ENTER);
    ARES_PCK_ID_NAME(0x74, ZC_REFUSE_ENTER);
    ARES_PCK_ID_NAME(0x21d, CZ_LESSEFFECT);
    ARES_PCK_ID_NAME(0x7d, CZ_NOTIFY_ACTORINIT);
    ARES_PCK_ID_NAME(0x7f, ZC_NOTIFY_TIME);
    ARES_PCK_ID_NAME(0x8c9, CZ_REQ_SCHEDULER_CASHITEM);
    ARES_PCK_ID_NAME(0x8ca, ZC_ACK_SCHEDULER_CASHITEM);
    ARES_PCK_ID_NAME(0x14f, CZ_REQ_GUILD_MENU);

#undef ARES_PCK_ID_NAME

  }
}
