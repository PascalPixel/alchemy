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
