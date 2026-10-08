#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "btlwork.h"
#include "system.h"


u32 BattleEventRuntime_Reset(void)
{
    struct BattleEventState *events;

    events = &BtlWk->events;
    events->phase = BATTLE_PLAYBACK_IDLE;
    events->queue.count = 0;
    events->event_index = 0;
    events->timer = 0;
    events->queue.target_index = 0;
    events->pending_cue = 0x86;
    events->flags = 0;
}

s32 BattleEventRuntime_WaitForReady(void)
{
    s32 state;
    struct BattleEventState *events;

    events = &BtlWk->events;
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
    Scheduler_RemoveCallback(BattleEvent_Playback);
    return BattleEventRuntime_Reset();
}
