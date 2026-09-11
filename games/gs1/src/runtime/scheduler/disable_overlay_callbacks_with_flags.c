#include "types.h"
#include "scene.h"
#include "abi/runtime/scheduler/disable_overlay_callbacks_with_flags.h"

s32 GameFlag_Clear(s32 flag_no);

s32 Scheduler_DisableOverlayCallbacksWithFlags(void)
{
    Sys_Check();
    Sys_Check2();
    GameFlag_Clear(0x166);
    return GameFlag_Clear(0x152);
}
