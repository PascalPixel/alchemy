#include "script_operands.h"

void Func_0800e280(struct ScriptOperands *state, s32 operation, s32 value)
{
    s32 result;

    if (operation == 0) {
        state->unsigned_halfword = value;
    } else if (operation == 1) {
        state->unsigned_halfword =
            (u16)((u32)state->unsigned_halfword + (u32)value);
    } else {
        result = 0;
        if (state->unsigned_halfword == (u16)value)
            result = 1;
        state->comparison_result = result;
    }
}
