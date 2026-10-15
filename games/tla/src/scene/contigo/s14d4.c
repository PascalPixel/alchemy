#include "contigo.h"

void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);

void Func_020014d4(void)
{
    if (!GameFlag_Test(62))
    {
        struct EventWork *work = Ram_HeapSlots->eventWork;

        Func_0200406c();
        Func_02004204(1);
        Func_020040d4(16, 664 << 16, 952 << 16);
        WaitFrames(1);
        ObjectMotion_SetAngleToward(16, gPartyState.currentOwner, 0);
        work->unk16a = 200;
        Func_02004074();
    }
}
