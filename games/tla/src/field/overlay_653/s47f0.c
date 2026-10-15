#include "local.h"

void Func_020046d8(void)
{
    Func_020051c0();
    Func_02005370(0);
    Func_02005230_(gPartyState.current_owner, 0, 0);
    Func_02005230_(12, 0, 0);
    Func_02005230_(15, 0, 0);
    Motion_CamBounds_(-1, -1, -1, 0);
    Func_020052f0()->motion_flags = 0;
    WaitFrames_(1);
    ((struct EventWork1ac *)Ram_HeapSlots->event_work)->value = 514;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_02005230_(12, 0x1d80000, 0x2880000);
    WaitFrames_(1);
    ObjectMotion_SetPositionAndReset_(12, 472, 664);
    ObjectMotion_ArmCallback_(12, 0x3000, 20);
    ObjectMotion_ArmCallback_(12, 0x5000, 20);
    ObjectMotion_ArmCallback_(12, 0x3000, 0);
    Func_020052d8(0x4cccc, 0x9999);
    Motion_CamBounds_(0x1c80000, 0x280000, 0x2980000, 1);
    Func_020052e8();
    Battle_WaitMode0_(40);
    Func_02005280((s32)&MsgScene653No06);
    Func_02005298(0x800c, 0);
    ObjectMotion_SetPositionAndReset_(12, 472, 648);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_020052f8(14);
}

void Func_020047f0(void)
{
    ObjectMotion_SetSpeedParameters_(12, 0x19999, 0xcccc);
    ObjectMotion_SetPositionAndReset_(12, 496, 392);
    ObjectMotion_ArmCallback_(12, 0x8000, 0);
    Func_02005298(0x400c, 0);
}
