#include "types.h"
#include "scene.h"
#include "abi/runtime/blank_display_and_run.h"

s32 Audio_PlayCue(s32);

s32 Runtime_BlankDisplayAndRun(void)
{
    *(s16 *)0x04000000 = 0x40;
    Audio_PlayCue(9);
    Sys_Check();
    return 0;
}
