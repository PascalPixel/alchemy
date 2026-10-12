#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "btlwork.h"
#include "system.h"
#include "callback_scheduler.h"

u32 BattleEventRuntime_Reset(void);

u32 BattleEventRuntime_Reset(void)
{
    struct BattleEventState *events;
    struct BtlWork *work;

    /* FAKEMATCH: the one-pass block loads the battle work before the events offset is built (differ 2 without it). */
    do {
        work = Ram_HeapSlots->battle_work;
    } while (0);
    events = &work->events;
    events->phase = BATTLE_PLAYBACK_IDLE;
    events->queue.count = 0;
    events->event_index = 0;
    events->timer = 0;
    events->queue.target_index = 0;
    events->pending_cue = 0x86;
    events->unknown_16a = 0;
    events->flags = 0;
}

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
