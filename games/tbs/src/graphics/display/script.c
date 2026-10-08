#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "global_cells.h"
#include "dma.h"
#include "types.h"
#include "callback_scheduler.h"
#include "system.h"
#include "map.h"
#include "palqueue.h"
#include "io_reg.h"


void DisplayBlend_RunScript(void)
{
    struct MapState *work = gMapWork[0];
    struct BlendScriptState *state;
    u16 *cursor;
    u32 command;
    u32 value;
    u16 test;

    state = &work->blend;
    if (state->script == 0)
        return;
    if (state->paused != 0)
        return;

again:
    value = state->delay;
    test = value;
    if (test != 0)
        goto tick;

    cursor = state->cursor;
    command = *cursor;
    cursor++;
    if (command == 0xffff) {
        state->cursor = state->script;
        goto again;
    }

    if ((command & 0xff00) == 0xfe00) {
        value = command & 0xff;
        if (value == 0xff)
            return;
        state->cursor = state->script + value * 2;
        goto again;
    }

    if ((command & 0xf000) == 0x3000) {
        REG_BLDCNT = command;
        work->blend_control = command;
        state->cursor++;
        goto again;
    }

    if ((work->blend_control & 0xc0) == 0x40)
        REG_BLDALPHA = command;
    else
        REG_BLDY = command;
    state->delay = cursor[0];
    state->cursor += 2;
    goto again;

tick:
    state->delay = value + 0xffff;
}

void DisplayBlend_StartScript(u16 *script)
{
    struct BlendScriptState *state;
    s32 started;
    volatile u32 zero;

    started = 0;
    zero = 0;
    state = &((struct MapState *)gMapWork[0])->blend;
    Dma_Set((const void *)&zero, state, 0x85000003, REG_DMA3);
    if (*script != 0xffff) {
        state->script = script;
        state->cursor = script;
        state->delay = 0;
        state->paused = 0;
        started = 1;
    }
    if (started)
        Scheduler_AddOrUpdateCallback(DisplayBlend_RunScript, 0xc80);
}

void DisplayBlend_EnableRunScript(void)
{
    Scheduler_EnableCallbacks(DisplayBlend_RunScript);
}

void DisplayBlend_DisableRunScript(void)
{
    Scheduler_DisableCallbacks(DisplayBlend_RunScript);
}

void Runtime_AllocateAndClearQueue(void)
{
    struct PaletteSnapshotWork *queue;
    struct PaletteSnapshot *entry;
    u16 i;
    u16 j;

    queue = Runtime_AllocateBlock(28, sizeof(struct PaletteSnapshotWork));
    entry = queue->entries;
    for (i = 0; i != 4; i++) {
        entry->source = 0;
        entry->position = 0;
        entry->timer = 0;
        entry->delay = 0;
        entry->count = 0;
        for (j = 0; j != 16; j++) {
            entry->colors[j] = 0;
        }
        entry++;
    }
    queue->count = 0;
}

s32 PaletteQueue_Add(s16 bank, s16 index, s16 delay, s16 count)
{
    struct PaletteSnapshotWork *work;
    struct PaletteSnapshot *entry;
    u16 *source;
    u32 slot;
    u16 size;

    work = gPaletteWork;
    slot = work->count;
    if (slot > 3)
        return -1;
    entry = &work->entries[slot];
    source = (u16 *)BG_PLTT + (((u16)bank << 4) + (u16)index);
    size = count;
    entry->position = 0;
    entry->timer = 0;
    entry->count = size;
    entry->source = source;
    entry->delay = delay;
    Dma_Set(source, entry->colors, 0x80000000 | size, REG_DMA3);
    work->count++;
    return 0;
}

void Runtime_ScheduleCallbackAndReleaseBlock28(void)
{
    Scheduler_RemoveCallback(PaletteQueue_StepCycles);
    Runtime_ReleaseHeapBlock(0x1C);
}
