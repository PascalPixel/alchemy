#include "RUNTIME_1E74.H"
#include "SYSTEM.H"

extern u8 Data_03001e74[];

void BattleEvent_Playback(void);

u32 BattleEventRuntime_Reset(void)
{
    struct Runtime1e74 *runtime;

    runtime = Runtime1e74_Get();
    runtime->phase = 0;
    runtime->value_7fc = 0;
    runtime->value_804 = 0;
    runtime->value_808 = 0;
    runtime->value_7f8 = 0;
    runtime->parameter = 0x86;
    runtime->value_824 = 0;
}

s32 BattleEventRuntime_WaitForReady(void)
{
    s32 state;
    struct Runtime1e74 *runtime;

    runtime = *(struct Runtime1e74 **)Data_03001e74;
    state = runtime->phase;
    if (state == 0) {
        runtime->phase = 1;
        state = 1;
    }
    if (state != 4) {
        do {
            WaitFrames(1U);
        } while (runtime->phase != 4);
    }
    Scheduler_RemoveCallback((u32)((void *)BattleEvent_Playback));
    return BattleEventRuntime_Reset();
}
