#include "TYPES.H"

s32 Func_080043e0(void);
s32 Func_08009298(void);
s32 GameFlag_Clear(s32 flag_no);

/* The owner that actually clears the overlay callbacks this routine then flags. */
#define Scheduler_DisableOverlayCallbacks Func_080043e0

s32 Scheduler_DisableOverlayCallbacksWithFlags(void)
{
    Scheduler_DisableOverlayCallbacks();
    Func_08009298();
    GameFlag_Clear(0x166);
    return GameFlag_Clear(0x152);
}
