#include "script_operands.h"

void Script_SetOrCompareCursor(struct ScriptOperands *state, s32 operation, s32 value)
{
    s32 result;

    if (operation == 0) {
        state->cursor = value;
    } else if (operation == 1) {
        state->cursor =
            (u16)((u32)(s32)(s16)state->cursor + (u32)value);
    } else {
        result = 0;
        if ((s16)state->cursor == (s16)value)
            result = 1;
        state->comparison_result = result;
    }
}
