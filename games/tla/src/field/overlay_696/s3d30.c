#include "ov696.h"

void Func_0200482c(void);

#define EVENT_WORK ((struct Ov696EventWork *)Ram_HeapSlots->event_work)

void Func_02003d30(void)
{
    s32 angle;

    GameFlag_ClearBit(0x11b);
    Func_020048c4();
    Func_02004a24(0);
    Func_02004934(16, 0, 0);
    Func_02004934(17, 0, 0);
    Object_GetById(15)->facing = 0xc000;
    Ov696_CamMove(0xe00000, -1, 0xb60000, 0);
    WaitFrames(1);
    Func_0200482c();
    WaitFrames(1);
    angle = 0x4000;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Ov696_ObjPut(16, 0xb80000, 0x800000, angle);
    Ov696_ObjPut(17, 0x1080000, 0x800000, angle);
    Ov696_ObjSpeed(16, 0x19999, 0xcccc);
    Ov696_ObjSpeed(17, 0x19999, 0xcccc);
    Ov696_CamSpeed(0xcccc, 0x1999);
    Ov696_CamMove(0xe00000, -1, 0x1180000, 1);
    Ov696_ObjWalk2(16, 184, 256);
    Ov696_ObjWalk(17, 264, 256);
    Ov696_ObjArm(gPartyState.current_owner, 0xe000, 0);
    Ov696_ObjWalk2(16, 208, 264);
    Ov696_ObjWalk(17, 240, 264);
    Object_SetModeById(16, 1);
    Battle_WaitMode0(10);
    Ov696_ObjArm(16, angle, 0);
    Ov696_ObjArm(17, angle, 0);
    Ov696_ObjTurn(15, 0x8000);
    Ov696_ObjArm(gPartyState.current_owner, 0, 0);
    Func_020049f4();
    Func_0200498c((s32)MsgShamanMustFinishTrialRoad);
    Ov696_MessagePut(0x200f, 0);
    Object_SetModeById(15, 3);
    Ov696_MessageOpen(0x200f, 0);
    if (Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0) == 1) {
        Ov696_MessageOpen(0x200f, 0);
        if (Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0) == 0) {
            Motion_SetModeAndWaitAnimation(15, 3);
            Engine_AudioPlayCue(76);
            Ov696_MessagePut(0x200f, 0);
            GameFlag_SetBit(0x200);
            Func_02001380();
            return;
        }
        EVENT_WORK->field_1c4++;
    } else {
        EVENT_WORK->field_1c4 += 2;
    }
    Ov696_MessageOpen(0x200f, 0);
    if (Inventory_PromptAndSetObjectMode(gPartyState.current_owner, 0) == 1) {
        Ov696_MessagePut(0x200f, 0);
        Func_0200289c();
    } else {
        EVENT_WORK->field_1c4++;
        Ov696_MessagePut(0x200f, 0);
        Func_0200290c();
    }
    Func_02004a2c();
    GameFlag_ClearBit(0x12f);
    Func_020048cc();
}
