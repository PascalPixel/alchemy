#include "types.h"
#include "scene.h"
#include "abi/runtime/scheduler/enable_overlay_callbacks_with_flags.h"

s32 GameFlag_Set(s32 flag_no);

s32 Scheduler_EnableOverlayCallbacksWithFlags(void)
{
    GameFlag_Set(0x152);
    GameFlag_Set(0x166);
    Sys_Check();
    return Sys_Check2();
}
