#include "btlwork.h"

u32 BattleEventRuntime_Reset(void)
{
    struct BattleEventState *events;

    events = &Ram_HeapSlots->battle_work->events;
    events->phase = BATTLE_PLAYBACK_IDLE;
    events->queue.count = 0;
    events->event_index = 0;
    events->timer = 0;
    events->queue.target_index = 0;
    events->pending_cue = 0x86;
    events->unknown_16a = 0;
    events->flags = 0;
}
