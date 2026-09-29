#include "NIWA.H"
extern u8 MsgBiribinoDoThinkCanBecomeAs[];
extern u8 MsgBiribinoTellingMeImResponsibleFor[];

void SceneDialogue_RunActor10Message13c3(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgBiribinoTellingMeImResponsibleFor);
    Event_AskYesNo(10, 0);
    Event_End();
}

void SceneDialogue_RunActor11Message1751(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgBiribinoDoThinkCanBecomeAs);
    Event_AskYesNo(11, 0);
    Event_End();
}
