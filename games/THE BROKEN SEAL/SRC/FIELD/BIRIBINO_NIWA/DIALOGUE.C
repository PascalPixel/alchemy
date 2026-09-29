#include "NIWA.H"
extern u8 MsgBiribinoDoThinkCanBecomeAs[];
extern u8 MsgBiribinoTellingMeImResponsibleFor[];
extern u8 MsgBiribinoHaveYouSeenBarricadeWe[];

void SceneDialogue_AskAboutBarricade(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgBiribinoHaveYouSeenBarricadeWe);
    Event_AskYesNo(9, 0);
    Event_End();
}

void SceneDialogue_AskIfResponsible(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgBiribinoTellingMeImResponsibleFor);
    Event_AskYesNo(10, 0);
    Event_End();
}

void SceneDialogue_AskIfFineWarrior(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgBiribinoDoThinkCanBecomeAs);
    Event_AskYesNo(11, 0);
    Event_End();
}
