#include "types.h"

#define Scheduler_DisableOverlayCallbacksWithFlags Func_080a1050

s32 Func_080043e0(void);
s32 Func_08009298(void);
s32 GameFlag_Clear(s32 flag_no);

s32 Scheduler_DisableOverlayCallbacksWithFlags(void)
{
    Func_080043e0();
    Func_08009298();
    GameFlag_Clear(0x166);
    return GameFlag_Clear(0x152);
}
