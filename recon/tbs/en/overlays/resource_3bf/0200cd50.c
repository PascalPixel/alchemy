/* Draft of ConfigureActor13Interaction, resource_3bf at 0x0200cd50, built with
 * games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/FORTRESS.H.
 * Remaining difference: the ROM keeps message 0x256c in r5 and adds 2 for the
 * second line, as it would a link-time value; GCC folds the C constant into a
 * second pool word. The listing keeps these rows. */
#include "FORTRESS.H"

void ConfigureActor13Interaction(void)
{
    u8 *interaction_resources = 0x256c;

    Event_SetMessage((s32)interaction_resources);
    Event_ShowMessage(0x800d, 0);
    if (PartyInventory_FindOwner(234) != -1) {
        Message_ShowCentered((s32)(interaction_resources + 2), 1);
    }
}
