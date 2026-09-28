/* Draft of resource_3ab 0x02008a90..0x02008ac4 (52 bytes with pool),
 * RightGuard_MindRead; the listing keeps the rows. Remaining difference: the
 * reference loads message 0x1bc0 from its literal pool, as a link-time
 * message symbol is loaded; the constant compiles to movs #222 / lsls #5 (11
 * bytes differ, 4 bytes shorter). */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/RUNPA_MURA/VILLAGE.H"

void RightGuard_MindRead(void)
{
    if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        Event_SetMessage(MSG_RIGHT_GUARD_REOPENED_THOUGHTS);
    } else {
        Event_SetMessage(MSG_RIGHT_GUARD_THOUGHTS);
    }
    Event_ShowMessage(ACTOR_RIGHT_GUARD, 0);
}
