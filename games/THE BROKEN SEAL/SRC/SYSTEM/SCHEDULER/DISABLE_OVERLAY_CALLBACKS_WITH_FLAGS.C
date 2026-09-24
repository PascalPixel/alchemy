#include "TYPES.H"

s32 Scheduler_DisableOverlayCallbacks(void);
s32 Map_DisableUpdateCallbackFar(void);
s32 GameFlag_ClearBitFar(s32 flag_no);

/* The owner that actually clears the overlay callbacks this routine then flags. */

s32 Scheduler_DisableOverlayCallbacksWithFlags(void)
{
    Scheduler_DisableOverlayCallbacks();
    Map_DisableUpdateCallbackFar();
    GameFlag_ClearBitFar(0x166);
    return GameFlag_ClearBitFar(0x152);
}
