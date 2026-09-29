/* Asks whether the would-be warrior remembers the speaker. */
#include "TOREBI.H"
extern u8 MsgTorebiWarriorRemember[];

void SceneDialogue_AskRememberWarrior(s32 subject)
{
    s32 message;

    Event_Begin();

    message = (s32)MsgTorebiWarriorRemember;
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
