/* Draft of SuharaSabaku_SelectEvents, resource_3c0 at 0x02008e5c, built with
 * games/THE BROKEN SEAL/SRC/FIELD/SUHARA_SABAKU/SABAKU.H.
 * Remaining difference: the ROM loads scene 0xa6 from its literal pool as a
 * link-time value; GCC compares an immediate.
 * The listing keeps these rows. */
#include "SABAKU.H"

s32 SuharaSabaku_SelectEvents(void)
{
    if (gGameState.scene == 0xa6) {
        return (s32)gSuharaSabakuEventsA6;
    }
    return (s32)gSuharaSabakuEvents;
}
