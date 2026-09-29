/* The Lunpa fortress: actor 13's lines. */
#include "FORTRESS.H"
extern u8 MsgRunpaCantBelieveWhen[];
extern u8 MsgRunpaWrongTurnOver[];

void ConfigureActor13Interaction(void)
{
    u8 *msg = (s32)MsgRunpaWrongTurnOver;

    Event_SetMessage((s32)msg);
    Event_ShowMessage(0x800d, 0);
    if (PartyInventory_FindOwner(234) != -1) {
        Message_ShowCentered((s32)(msg + 2), 1);
    }
}

void ConfigureActor13SceneResource(void)
{
    Event_SetMessage((s32)MsgRunpaCantBelieveWhen);
    Event_ShowMessage(13, 0);
}
