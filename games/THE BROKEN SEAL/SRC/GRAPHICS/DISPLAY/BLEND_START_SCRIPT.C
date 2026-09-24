#include "DMA.H"

struct BlendScriptState {
    u16 *script;
    u16 *cursor;
    u16 delay;
    u16 paused;
};

struct FieldCells {
    u8 unknown_00[0xd8];
    struct BlendScriptState blend;
};

extern struct FieldCells *gMapWork;

void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 flags);
void DisplayBlend_RunScript(void);

/* Clears the blend script state and, unless the script is empty (0xffff),
   starts it and schedules DisplayBlend_RunScript. */
void DisplayBlend_StartScript(u16 *script)
{
    struct BlendScriptState *state;
    s32 started;
    volatile u32 zero;

    started = 0;
    zero = 0;
    state = &gMapWork->blend;
    Dma_Set((const void *)&zero, state, 0x85000003, (volatile u32 *)0x040000d4);
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
