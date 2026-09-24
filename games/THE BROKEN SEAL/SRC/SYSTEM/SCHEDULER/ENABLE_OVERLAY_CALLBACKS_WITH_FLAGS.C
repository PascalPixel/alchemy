#include "TYPES.H"

s32 Scheduler_EnableUnmaskedOverlayCallbacks(void);
s32 Map_EnableUpdateCallbackFar(void);
s32 GameFlag_Set(s32 flag_no);

s32 Scheduler_EnableOverlayCallbacksWithFlags(void)
{
    GameFlag_Set(0x152);
    GameFlag_Set(0x166);
    Map_EnableUpdateCallbackFar();
    return Scheduler_EnableUnmaskedOverlayCallbacks();
}
