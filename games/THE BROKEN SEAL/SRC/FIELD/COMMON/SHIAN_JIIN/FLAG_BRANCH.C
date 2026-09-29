#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "TEMPLE.H"
extern u8 MsgShianHowDidGetHereBridge[];
extern u8 MsgShianIsntNobleHimTrySave[];
extern u8 MsgShianNowHeTrulyBeyondWorlds[];

void FieldScene_RunScene39e_02000414(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage((s32)MsgShianHowDidGetHereBridge);
    if (GameFlag_IsSet(0x890) != 0) {
        bump_step(4);
    }
    Event_OpenMessage(8, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        GameFlag_Set(0x890);
    } else {
        bump_step(1);
    }
    Event_ShowMessage(8, 0);
    Event_End();
}

void FieldScene_RunFlag88FBranch(void)
{
    extern u8 *gWork;

    Event_Begin();
    if (GameFlag_IsSet(0x88F) != 0) {
        Event_SetMessage((s32)MsgShianNowHeTrulyBeyondWorlds);
        Event_AskYesNo(12, 0);
        Event_End();
    } else {
        Event_SetMessage((s32)MsgShianIsntNobleHimTrySave);
        Event_OpenMessage(12, 0);
        if (Event_ChooseYesNo(0, 0) == 1) {
            u16 *q = (u16 *)(gWork + 0x1D8);
            q[0] = q[0] + 1;
            Event_OpenMessage(12, 0);
            if (Event_ChooseYesNo(0, 0) == 1) {
                u16 *r = (u16 *)(gWork + 0x1D8);
                r[0] = r[0] + 1;
            }
        }
        Event_ShowMessage(12, 0);
        Event_End();
    }
}
