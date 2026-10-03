#include "BATTLE_WORK.H"
#include "SYSTEM.H"

void BattleEvent_Playback(void);

u32 BattleEventRuntime_Reset(void)
{
    struct BattleSession *runtime;

    runtime = gBattleWork;
    runtime->events.phase = BATTLE_PLAYBACK_IDLE;
    runtime->events.queue.count = 0;
    runtime->events.event_index = 0;
    runtime->events.timer = 0;
    runtime->events.queue.target_index = 0;
    runtime->events.pending_cue = 0x86;
    runtime->events.flags = 0;
}

s32 BattleEventRuntime_WaitForReady(void)
{
    s32 state;
    struct BattleSession *runtime;

    runtime = gBattleWork;
    state = runtime->events.phase;
    if (state == BATTLE_PLAYBACK_IDLE) {
        runtime->events.phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
        state = BATTLE_PLAYBACK_RESOLVE_TARGET;
    }
    if (state != BATTLE_PLAYBACK_READY) {
        do {
            WaitFrames(1U);
        } while (runtime->events.phase != BATTLE_PLAYBACK_READY);
    }
    Scheduler_RemoveCallback((u32)((void *)BattleEvent_Playback));
    return BattleEventRuntime_Reset();
}
