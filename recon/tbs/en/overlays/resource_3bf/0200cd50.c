/* Draft of resource_3bf 0x0200cd50 (ConfigureActor13Interaction): it matches
 * the ROM byte for byte now that the message it loads from the literal pool
 * has a catalogue name (MsgRunpaWrongTurnOver). The listing keeps these rows
 * until the draft is adopted. */
#include "FORTRESS.H"
extern u8 MsgRunpaWrongTurnOver[];

void ConfigureActor13Interaction(void)
{
    u8 *interaction_resources = (s32)MsgRunpaWrongTurnOver;

    Event_SetMessage((s32)interaction_resources);
    Event_ShowMessage(0x800d, 0);
    if (PartyInventory_FindOwner(234) != -1) {
        Message_ShowCentered((s32)(interaction_resources + 2), 1);
    }
}
