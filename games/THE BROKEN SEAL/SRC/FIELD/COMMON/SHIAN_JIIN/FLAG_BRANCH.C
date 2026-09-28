#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "TEMPLE.H"

void FieldScene_RunScene39e_02000414(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage(MSG_HOW_DID_GET_HERE_BRIDGE);
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
        Event_SetMessage(MSG_NOW_HE_TRULY_BEYOND_WORLDS);
        Event_AskYesNo(12, 0);
        Event_End();
    } else {
        Event_SetMessage(MSG_ISNT_NOBLE_HIM_TRY_SAVE);
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
