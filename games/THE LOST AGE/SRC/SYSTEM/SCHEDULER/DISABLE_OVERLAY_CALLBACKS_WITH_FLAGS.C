#include "TYPES.H"

s32 Scheduler_DisableOverlayCallbacks(void);
s32 Scheduler_EnableUnmaskedOverlayCallbacks(void);
s32 Map_DisableUpdateCallbackFar(void);
s32 Map_EnableUpdateCallbackFar(void);
s32 GameFlag_ClearBit(s32 flag_no);
s32 GameFlag_SetBit(s32 flag_no);

/* The owner that actually clears the overlay callbacks this routine then flags. */

s32 Scheduler_DisableOverlayCallbacksWithFlags(void)
{
    Scheduler_DisableOverlayCallbacks();
    Map_DisableUpdateCallbackFar();
    GameFlag_ClearBit(0x166);
    return GameFlag_ClearBit(0x152);
}

s32 Scheduler_EnableOverlayCallbacksWithFlags(void)
{
    GameFlag_SetBit(0x152);
    GameFlag_SetBit(0x166);
    Map_EnableUpdateCallbackFar();
    return Scheduler_EnableUnmaskedOverlayCallbacks();
}
