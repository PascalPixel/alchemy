#include "shaman_village.h"

void Func_02003b6c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

void Func_02003ce4(void)
{
    if (GameFlag_Test(0x202)) {
        Func_020048c4();
        Func_02004a24(0);
        Engine_AudioPlayCue(123);
        Event_FinishScreenTransition();
        Event_WaitTransitionFrames();
        Func_02004a04(13);
    } else {
        Func_02003b6c(43, 40, 7, 6, 1, 2);
    }
}
