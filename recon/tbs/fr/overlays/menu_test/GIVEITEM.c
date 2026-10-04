/* Former one-argument attempt: 12 bytes against the complete 16-byte
 * callback. 2026-10-04: native direct and field callers corroborate an unused
 * second argument. TEST.C now uses the shared two-argument contract and an
 * ordinary call; this older measurement remains as the failed attempt. */
#include "TYPES.H"
s32 PartyInventory_GiveItem(s32 item);
void DebugMenu_GiveItemToParty(void)
{
    PartyInventory_GiveItem(181);
}
