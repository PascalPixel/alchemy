#include "TYPES.H"
#include "DMA.H"
#include "CALLBACK_SCHEDULER.H"
#include "RAM_BUFFER.H"
#include "MAP.H"

/* Blend script runner: a halfword script in the map work that writes
   BLDCNT and the alpha or brightness level on a frame delay. */
void DisplayBlend_RunScript(void)
{
    struct MapWork *work = Ram_HeapSlots->map_work;
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
        *(volatile u16 *)0x04000050 = command;
        work->blend_control = command;
        state->cursor++;
        goto again;
    }

    if ((work->blend_control & 0xc0) == 0x40)
        *(volatile u16 *)0x04000052 = command;
    else
        *(volatile u16 *)0x04000054 = command;
    state->delay = cursor[0];
    state->cursor += 2;
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
    u32 first;

    started = 0;
    zero = 0;
    state = &Ram_HeapSlots->map_work->blend;
    Dma_Set((const void *)&zero, state, 0x85000003, (volatile u32 *)0x040000d4);
    first = *script;
    /* FAKEMATCH: the plain order builds 0xffff before loading the first command. */
    __asm__("");
    if (first != 0xffff) {
        state->script = script;
        state->cursor = script;
        state->delay = 0;
        state->paused = 0;
        started = 1;
    }
    if (started)
        Scheduler_AddOrUpdateCallback((s32)(DisplayBlend_RunScript), 0x480);
}

void DisplayBlend_EnableRunScript(void)
{
    Scheduler_EnableCallbacks((u32)DisplayBlend_RunScript);
}

void DisplayBlend_DisableRunScript(void)
{
    Scheduler_DisableCallbacks((u32)DisplayBlend_RunScript);
}
