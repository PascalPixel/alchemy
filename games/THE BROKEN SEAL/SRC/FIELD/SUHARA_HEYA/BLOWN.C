/* Asks whether the party was blown here on the way to Babi Lighthouse, with a
 * line for each answer. */
#include "SUHARA.H"
extern u8 MsgSuharaSupposeFolkBlown[];

void SuharaHeya_AskBlownHere(s32 obj)
{
    s32 msg = (s32)MsgSuharaSupposeFolkBlown;
    Event_SetMessage(msg);
    Event_OpenMessage(obj, 0);
    if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
        Event_Wait(10);
        Event_SetMessage(msg + 1);
    } else {
        Event_SetMessage(msg + 2);
    }
    Event_ShowMessage(obj, 0);
}
