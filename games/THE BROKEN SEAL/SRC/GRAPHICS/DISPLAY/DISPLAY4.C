#include "GLOBAL_CELLS.H"
#include "DMA.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "MAP.H"


struct PaletteSnapshot {
    u16 *source;
    u16 unknown_04;
    u16 unknown_06;
    s16 value;
    u16 count;
    u16 colors[16];
};

struct PaletteSnapshotWork {
    struct PaletteSnapshot entries[4];
    u16 count;
};

extern struct PaletteSnapshotWork *gPaletteWork;

void Func_08011bf4(void);

/* Blend script runner: a halfword script in the map work that writes
   BLDCNT and the alpha or brightness level on a frame delay. */
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
        state->cursor = (u16 *)((u8 *)state->script + value * 4);
        goto again;
    }

    if ((command & 0xf000) == 0x3000) {
        *(volatile u16 *)0x04000050 = command;
        work->blend_control = command;
        state->cursor = (u16 *)((u8 *)state->cursor + 2);
        goto again;
    }

    if ((work->blend_control & 0xc0) == 0x40)
        *(volatile u16 *)0x04000052 = command;
    else
        *(volatile u16 *)0x04000054 = command;
    state->delay = cursor[0];
    state->cursor = (u16 *)((u8 *)state->cursor + 4);
    goto again;

tick:
    state->delay = value + 0xffff;
}

/* Clears the blend script state and, unless the script is empty (0xffff),
   starts it and schedules DisplayBlend_RunScript. */
void DisplayBlend_StartScript(u16 *script)
{
    struct BlendScriptState *state;
    s32 started;
    volatile u32 zero;

    started = 0;
    zero = 0;
    state = &((struct MapState *)gMapWork[0])->blend;
    Dma_Set((const void *)&zero, state, 0x85000003, (volatile u32 *)0x040000d4);
    if (*script != 0xffff) {
        state->script = script;
        state->cursor = script;
        state->delay = 0;
        state->paused = 0;
        started = 1;
    }
    if (started)
        Scheduler_AddOrUpdateCallback((s32)(DisplayBlend_RunScript), 0xc80);
}

void DisplayBlend_EnableRunScript(void)
{
    Scheduler_EnableCallbacks((u32)DisplayBlend_RunScript);
}

void DisplayBlend_DisableRunScript(void)
{
    Scheduler_DisableCallbacks((u32)DisplayBlend_RunScript);
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
        entry->unknown_04 = 0;
        entry->unknown_06 = 0;
        entry->value = 0;
        entry->count = 0;
        for (j = 0; j != 16; j++) {
            entry->colors[j] = 0;
        }
        entry++;
    }
    queue->count = 0;
}

/* Queues up to four palette snapshots (the queue Runtime_AllocateAndClearQueue
   clears): copies count colours of palette bank:index into the next slot. */
s32 PaletteQueue_Add(s16 bank, s16 index, s16 value, s16 count)
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
    source = (u16 *)0x05000000 + (((u16)bank << 4) + (u16)index);
    size = count;
    entry->unknown_04 = 0;
    entry->unknown_06 = 0;
    entry->count = size;
    entry->source = source;
    entry->value = value;
    Dma_Set(source, entry->colors, 0x80000000 | size, (volatile u32 *)0x040000d4);
    work->count++;
    return 0;
}

void Runtime_ScheduleCallbackAndReleaseBlock28(void)
{
    Scheduler_RemoveCallback((u32)Func_08011bf4);
    Runtime_ReleaseHeapBlock(0x1C);
}
