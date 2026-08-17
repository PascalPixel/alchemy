#include "resource_37c.h"
#include "resource_37c_runtime.h"

#define Resource37c_Run Func_02000054

s32 Resource37c_Run(void)
{
    /* Keep distinct entry-block 16.16 scale locals: local CSE otherwise
       replaces two materializations with register copies. */
    s32 scale_x = 0x10000;
    s32 scale_y = 0x10000;
    s32 scale_z = 0x10000;

    RESOURCE37C_RUNTIME->setup_request_1c0 = 0x204;
    Resource37c_SetSetupFlag(0x144);
    if (Resource37c_TestSetupGate(0x814) != 0) {
        Resource37c_QueueSoundCue(0x8D);
        Resource37c_SetViewScale(scale_x, scale_y, scale_z);
        Resource37c_FinalizeSetup();
    }
    return 0;
}
