#include "ov670.h"

void Func_02000c90(void)
{
    struct Ov670Object *leader;

    Func_02005a9c();
    Func_02005c3c(0);
    Ov670_ObjArm(gPartyState.current_owner, 0, 0);
    Func_02005ba4(0x26666, 0x4ccc);
    Ov670_CamMove(0x1180000, -1, 0x580000, 1);
    Func_02005bb4();
    Battle_WaitMode0(20);
    Func_02005b4c((s32)MsgYouVeCome);
    Func_02005b64(0x8007, 0);
    Ov670_ObjTurn(7, 0x8000);
    Motion_SetModeAndWaitAnimation(gPartyState.current_owner, 3);
    Motion_SetModeAndWaitAnimation(7, 3);
    Ov670_ObjSpeed(7, 0x19999, 0xcccc);
    Object_SetModeById(7, 2);
    leader = Object_GetById(gPartyState.current_owner);
    if (leader != 0)
        ObjectMotion_ResetAndSetPosition(7, *(s16 *)((u8 *)leader + 10), *(s16 *)((u8 *)leader + 18));
    ObjectMotion_CommitCurrentPositionAndActivate(7);
    Func_02005b0c(7, 0, 0);
    GameFlag_ClearBit(0x92f);
    GameFlag_SetBit(0x92e);
    Event_PrepareObjectAndApplyValue(7, 0);
    Func_02005aa4();
}
