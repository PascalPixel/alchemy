#include "script_operands.h"

void Func_0800e720(struct ScriptOperands *state, s32 operation, s32 value)
{
    s32 result;
    if (operation == 0) {
        state->byte_59 = value;
    } else if (operation == 1) {
        state->byte_59 = (u8)((u32)state->byte_59 + (u32)value);
    } else {
        result = 0;
        if (state->byte_59 == (u8)value)
            result = 1;
        state->comparison_result = result;
    }
}
