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

void BattleEventRuntime_BeginPhase(s32 cue)
{
    struct BattleEventState *events;

    events = &gBattleWork->events;
    if (events->phase == BATTLE_PLAYBACK_IDLE) {
        events->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
        if (cue != 0) {
            events->pending_cue = cue;
        }
    }
}

s32 BattleEventRuntime_SchedulePhase(s32 frames)
{
    struct BattleEventState *events;

    events = &gBattleWork->events;
    events->queue.count = 0;
    events->event_index = 0;
    events->timer = frames;
    events->phase = BATTLE_PLAYBACK_DISPATCH;
    gBattleWork->plan.target_count = 0;
    return Scheduler_AddOrUpdateCallback((s32)BattleEvent_Playback, 0xc80);
}
