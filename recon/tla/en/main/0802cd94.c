
/* Blend script runner: a halfword script in the map work that writes
   BLDCNT and the alpha or brightness level on a frame delay. */

struct BlendScriptState {
    u16 *script;
    u16 *cursor;
    u16 delay;
    u16 paused;
};

struct FieldCells {
    u8 unknown_00[0xd8];
    struct BlendScriptState blend;   /* 0xd8 */
    u8 unknown_e4[0x1f];
    u8 blend_control;                /* 0x103: last BLDCNT written */
};

extern struct FieldCells *gMapWork;

void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 flags);
s32 Scheduler_EnableCallbacks(u32 value);
s32 Scheduler_DisableCallbacks(u32 value);

void DisplayBlend_RunScript(void)
{
    struct FieldCells *work = gMapWork;
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
