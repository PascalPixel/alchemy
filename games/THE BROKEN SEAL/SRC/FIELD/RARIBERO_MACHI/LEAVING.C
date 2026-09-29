/* Asks whether the party is leaving Lalivero, with a line for each answer. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
extern u8 MsgRariberoLeavingLalivero[];

void RariberoMachi_AskLeaving(s32 obj)
{
    s32 msg = (s32)MsgRariberoLeavingLalivero;
    Event_SetMessage(msg);
    Event_OpenMessage(obj, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Event_SetMessage(msg + 1);
    } else {
        Event_SetMessage(msg + 2);
    }
    Event_ShowMessage(obj, 0);
}
