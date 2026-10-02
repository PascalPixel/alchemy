#include "DMA.H"
#include "BATTLE_WORK.H"

/* The container-built _call_via_r0 veneer: it calls the routine in r0. */
void _call_via_r0(u32 routine);

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
    struct BattleSession *runtime;

    runtime = gBattleWork;
    if (runtime->events.phase == 0) {
        runtime->events.phase = 1;
        if (parameter != 0) {
            runtime->events.pending_cue = parameter;
        }
    }
}

s32 BattleEventRuntime_SchedulePhase(s32 parameter)
{
    struct BattleSession *runtime;

    runtime = gBattleWork;
    runtime->events.queue.count = 0;
    runtime->events.event_index = 0;
    runtime->events.timer = parameter;
    runtime->events.phase = 2;
    runtime->plan.target_count = 0;
    return Scheduler_AddOrUpdateCallback((s32)BattleEvent_Playback, 0xC80);
}
