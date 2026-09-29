#include "KAREI.H"

void SceneDialogue_ShowLine1CF8(void)
{
    Event_Begin();
    Event_SetMessage(0x1CF8);
    Event_AskYesNo(8, 0);
    Event_End();
}

void FieldScene_RunScene3ae_02000260(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x8a6) == 0) {
        Event_SetMessage(0x1cfd);
        Event_OpenMessage(11, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_ShowMessage(11, 0);
            GameFlag_Set(0x8a6);
            goto L_020002c2;
        }
        bump_step(1);
        Event_ShowMessage(11, 0);
    } else {
        Event_SetMessage(0x1cfe);
        Event_ShowMessage(11, 0);
    }
    L_020002c2:;
    Event_End();
}

void FieldScene_RunScene3ae_020002dc(void)
{
    u32 i;
    s32 record;

    ((void)Engine_ActorGet(0));
    Event_Begin();
    if (GameFlag_IsSet(0x8a7) != 0) {
        if (GameFlag_IsSet(0x8a9) != 0) {
            Event_SetMessage(0x1d23);
            Event_OpenMessage(12, 0);
            Actor_FaceDirection(12, 0x4000, 0);
        }
    }
}
