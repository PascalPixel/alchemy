#include "PROTO.H"
#include "BATTLE_WORK.H"
#include "SYSTEM.H"
#include "CALLBACK_SCHEDULER.H"

u32 BattleEventRuntime_Reset(void);

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
