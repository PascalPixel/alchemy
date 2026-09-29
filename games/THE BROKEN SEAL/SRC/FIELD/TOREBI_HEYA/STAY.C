/* The offer to stay, and the excited girl's lines. */
#include "TOREBI.H"
extern u8 MsgTorebiHeeHeeLook[];
extern u8 MsgTorebiHello[];
extern u8 MsgTorebiHoHumFine[];
extern u8 MsgTorebiWantStay[];

void SceneDialogue_AskStay(s32 subject)
{
    s32 msg;

    Event_Begin();

    msg = (s32)MsgTorebiWantStay;
    Event_SetMessage(msg);
    Event_OpenMessage(subject, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(10);
        Event_SetMessage(msg + 1);
    } else {
        Event_SetMessage(msg + 2);
    }

    Event_ShowMessage(subject, 0);
    Event_End();
}

void SceneDialogue_RunExcitedLines(s32 a0)
{
    s32 msg;

    Event_Begin();
    if (GameFlag_IsSet(0x8bd) == 0) {
        msg = (s32)MsgTorebiHeeHeeLook;
        Event_SetMessage(msg);
        Event_OpenMessage(a0, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_Wait(10);
            Event_SetMessage(msg + 1);
        } else {
            Event_SetMessage(msg + 2);
        }
        Event_ShowMessage(a0, 0);
    } else {
        if (GameFlag_IsSet(0x8be) == 0) {
            GameFlag_Set(0x8be);
            Event_SetMessage((s32)MsgTorebiHello);
            Event_ShowMessage(a0, 0);
            Event_Wait(10);
            Actor_RunRepeatedMotion(a0, 2);
            Event_Wait(20);
        }
        Event_SetMessage((s32)MsgTorebiHoHumFine);
        Event_ShowMessage(a0, 0);
    }
    Event_End();
}
