#include "types.h"
#include "io_reg.h"

s32 Scheduler_EnableCallbacks(u32 callback);
void Object_EffectSpawnCallback(void);

void EventRuntime_UpdateWaitMode(void)
{
    struct BattleRuntime *runtime = Data_03001ebc;

    if (gDebugMode != 0) {
        if (gKeyState & KEY_L) {
            runtime->mode_1cc = 0;
        }
        if (gKeyState & KEY_R) {
            runtime->mode_1cc = -1;
        }
    }
}
