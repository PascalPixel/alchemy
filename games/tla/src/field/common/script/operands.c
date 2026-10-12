#include "script.h"

void Script_SetOrCompareAddress(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->script_address = value;
        return;
    }
    if (operation == 1) {
        state->script_address += value * 4;
        return;
    }
    result = 0;
    if (state->script_address == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareCursor(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == 0) {
        state->cursor = value;
    } else if (operation == 1) {
        state->cursor = (s16)state->cursor + value;
    } else {
        state->comparison_result = (s16)state->cursor == (s16)value;
    }
}

void Script_SetOrCompareUnsignedHalfword(struct ScriptOperands *state, s32 operation, s32 value)
{
    s32 result;

    if (operation == 0) {
        state->unsigned_halfword = value;
    } else if (operation == 1) {
        state->unsigned_halfword =
            (u16)((u32)state->unsigned_halfword + (u32)value);
    } else {
        u32 current = state->unsigned_halfword;

        asm volatile(""); /* FAKEMATCH: ⚓️ loads the halfword before it starts truncating the value; without the barrier the shift is scheduled first (differ 2) */
        result = 0;
        if (current == (u16)value)
            result = 1;
        state->comparison_result = result;
    }
}
