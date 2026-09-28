#include "NIWA.H"

void SceneDialogue_RunActor10Message13c3(void)
{
    Event_Begin();
    Event_SetMessage(MSG_TELLING_ME_IM_RESPONSIBLE_FOR);
    Event_AskYesNo(10, 0);
    Event_End();
}

void SceneDialogue_RunActor11Message1751(void)
{
    Event_Begin();
    Event_SetMessage(MSG_DO_THINK_CAN_BECOME_AS);
    Event_AskYesNo(11, 0);
    Event_End();
}
