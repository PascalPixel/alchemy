#include "script_operands.h"

void Script_SetOrCompareAddress50(struct ScriptOperands *state, s32 operation, s32 value) {
    s8 result;

    if (operation == 0) {
        state->address_50 = value;
        return;
    }
    if (operation == 1) {
        state->address_50 += (u32)value * 4;
        return;
    }
    result = 0;
    if (state->address_50 == (u32)value) {
        result = 1;
    }
    state->comparison_result = result;
}
