#include "DMA.H"
#include "RUNTIME_1E74.H"

/* The container-built _call_via_r0 veneer: it calls the routine in r0. */
void _call_via_r0(u32 routine);

extern u8 Data_03001e74[];

s32 Scheduler_AddOrUpdateCallback(s32, s32);
void BattleEvent_Playback(void);

/* Writes three empty transfers to the DMA3 registers, then runs the hook
   stored at 0x030000c4. */
void Dma_StopAllThenRunHook(void)
{
    Dma_Set(0, 0, 0x84000000, (volatile u32 *)0x040000d4);
    Dma_Set(0, 0, 0x84000000, (volatile u32 *)0x040000d4);
    Dma_Set(0, 0, 0x84000000, (volatile u32 *)0x040000d4);
    _call_via_r0(*(u32 *)0x030000c4);
}

void BattleEventRuntime_BeginPhase(s32 parameter)
{
    struct Runtime1e74 *runtime;

    runtime = Runtime1e74_Get();
    if (runtime->phase == 0) {
        runtime->phase = 1;
        if (parameter != 0) {
            runtime->parameter = parameter;
        }
    }
}

s32 BattleEventRuntime_SchedulePhase(s32 parameter)
{
    struct Runtime1e74 *runtime;

    runtime = Runtime1e74_Get();
    runtime->value_7fc = 0;
    runtime->value_804 = 0;
    runtime->value_808 = parameter;
    runtime->phase = 2;
    runtime->flag_655 = 0;
    return Scheduler_AddOrUpdateCallback((s32)BattleEvent_Playback, 0xC80);
}
