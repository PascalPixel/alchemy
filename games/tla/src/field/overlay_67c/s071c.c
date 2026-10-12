#include "local.h"

void Func_0200071c(void)
{
    s32 priority;

    /* FAKEMATCH: the native builds the priority before loading the callback. As a plain constant the load comes before the priority shift (differ 2); the same barrier after the assignment fixes that and moves the angle shift of the call before (differ 2). Naming the priority here, before it is set, keeps both, as in Func_02003628. */
    __asm__ volatile("" : "+r"(priority));
    Func_02003d7c();
    Func_02003eac(0);
    ObjectMotion_ArmCallback_(gPartyState.current_owner, 0x4000, 0);
    Func_02003ef4(0x22d);
    priority = 0x480;
    Scheduler_AddOrUpdateCallback(Func_020006d0, priority);
    Func_02003e44_(0x28000, 0x4000);
    Motion_CamBounds_(0x3080000, -1, 0x2880000, 1);
    Func_02003e54();
    Battle_WaitMode0(30);
    Func_02003d84();
    GameFlag_SetBit(0x240);
    Func_02003d1c_(59, 16, 5, 32, 46, 16);
}
