/* Draft: the ordinary one-argument item call omits the obsolete r1 zero; the original complete callback is16 bytes, this candidate12. */
#include "TYPES.H"
s32 PartyInventory_GiveItem(s32 item);
void DebugMenu_GiveItemToParty(void)
{
    PartyInventory_GiveItem(181);
}
