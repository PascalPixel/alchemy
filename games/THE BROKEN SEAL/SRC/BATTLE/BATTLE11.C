#include "BATTLE_WORK.H"
#include "SYSTEM.H"

void BattleEvent_Playback(void);

u32 BattleEventRuntime_Reset(void)
{
    struct BattleSession *runtime;

    runtime = gBattleWork;
    runtime->events.phase = 0;
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
    if (state == 0) {
        runtime->events.phase = 1;
        state = 1;
    }
    if (state != 4) {
        do {
            WaitFrames(1U);
        } while (runtime->events.phase != 4);
    }
    Scheduler_RemoveCallback((u32)((void *)BattleEvent_Playback));
    return BattleEventRuntime_Reset();
}
