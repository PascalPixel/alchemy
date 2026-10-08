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
