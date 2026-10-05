#include "TYPES.H"
s32 PartyInventory_GiveItem(s32 item);
void DebugMenu_GiveItemToParty(void)
{
    PartyInventory_GiveItem(181);
}
