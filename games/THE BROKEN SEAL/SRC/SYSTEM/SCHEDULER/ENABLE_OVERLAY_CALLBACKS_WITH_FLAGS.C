#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"

s32 Map_EnableUpdateCallbackFar(void);
s32 GameFlag_SetBitFar(s32 flag_no);

s32 Scheduler_EnableOverlayCallbacksWithFlags(void)
{
    GameFlag_SetBitFar(0x152);
    GameFlag_SetBitFar(0x166);
    Map_EnableUpdateCallbackFar();
    return Scheduler_EnableUnmaskedOverlayCallbacks();
}
