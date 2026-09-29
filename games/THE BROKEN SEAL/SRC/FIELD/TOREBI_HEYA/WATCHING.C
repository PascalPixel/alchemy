/* Asks whether the party was watching Colosso. */
#include "TOREBI.H"
extern u8 MsgTorebiLookLikeWarrior[];

void SceneDialogue_AskWatchingColosso(s32 subject)
{
    s32 message;

    Event_Begin();

    message = (s32)MsgTorebiLookLikeWarrior;
    Event_SetMessage(message);
    Event_OpenMessage(subject, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Event_SetMessage(message + 1);
    } else {
        Event_SetMessage(message + 2);
    }

    Event_ShowMessage(subject, 0);
    Event_End();
}
