#include "script_operands.h"

void Func_0800e308(struct ScriptOperands *state, s32 operation, s32 value) {
    s8 result;

    if (operation == 0) {
        state->word_10 = value;
        return;
    }
    if (operation == 1) {
        state->word_10 += value;
        return;
    }
    result = 0;
    if (state->word_10 == value) {
        result = 1;
    }
    state->comparison_result = result;
}
