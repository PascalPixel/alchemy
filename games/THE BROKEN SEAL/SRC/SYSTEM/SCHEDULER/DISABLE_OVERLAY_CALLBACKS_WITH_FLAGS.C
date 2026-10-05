#include "PROTO.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"

s32 GameFlag_ClearBitFar(s32 flag_no);

s32 Scheduler_DisableOverlayCallbacksWithFlags(void)
{
    Scheduler_DisableOverlayCallbacks();
    Map_DisableUpdateCallbackFar();
    GameFlag_ClearBitFar(0x166);
    return GameFlag_ClearBitFar(0x152);
}
