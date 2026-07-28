#include "script_operands.h"

void Func_0800e24c(struct ScriptOperands *state, s32 operation, s32 value)
{
    s32 result;

    if (operation == 0) {
        state->signed_halfword = value;
    } else if (operation == 1) {
        state->signed_halfword =
            (s16)((u32)(s32)state->signed_halfword + (u32)value);
    } else {
        result = 0;
        if (state->signed_halfword == (s16)value)
            result = 1;
        state->comparison_result = result;
    }
}
