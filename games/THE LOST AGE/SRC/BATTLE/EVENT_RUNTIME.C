#include "BATTLE_WORK.H"
#include "SYSTEM.H"
#include "CALLBACK_SCHEDULER.H"

void BattleEvent_Playback(void);
u32 BattleEventRuntime_Reset(void);

/* Start idle playback, wait until it is ready, then stop and reset it. */
s32 BattleEventRuntime_WaitForReady(void)
{
    s32 state;
    struct BattleEventState *events;

    events = &Ram_HeapSlots->battle_work->events;
    state = events->phase;
    if (state == BATTLE_PLAYBACK_IDLE) {
        events->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
        state = BATTLE_PLAYBACK_RESOLVE_TARGET;
    }
    if (state != BATTLE_PLAYBACK_READY) {
        do {
            WaitFrames(1U);
        } while (events->phase != BATTLE_PLAYBACK_READY);
    }
    Scheduler_RemoveCallback((u32)BattleEvent_Playback);
    return BattleEventRuntime_Reset();
}
