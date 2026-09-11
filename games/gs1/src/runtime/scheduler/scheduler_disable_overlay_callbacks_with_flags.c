#include "types.h"
#include "scene.h"

/* runtime/scheduler/disable_overlay_callbacks_with_flags.c */
s32 GameFlag_Clear(s32 flag_no);

s32 Scheduler_DisableOverlayCallbacksWithFlags(void)
{
    Sys_Check();
    Scheduler_EnableUnmaskedOverlayCallbacks();
    GameFlag_Clear(0x166);
    return GameFlag_Clear(0x152);
}

/* runtime/scheduler/enable_overlay_callbacks_with_flags.c */
s32 GameFlag_Set(s32 flag_no);

s32 Scheduler_EnableOverlayCallbacksWithFlags(void)
{
    GameFlag_Set(0x152);
    GameFlag_Set(0x166);
    Sys_Check();
    return Scheduler_EnableUnmaskedOverlayCallbacks();
}
