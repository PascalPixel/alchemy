#include "SCRIPT_OPERANDS.H"

void Script_SetOrCompareWord08(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_08 = value;
        return;
    }
    if (operation == 1) {
        state->word_08 += value;
        return;
    }
    result = 0;
    if (state->word_08 == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord0c(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_0c = value;
        return;
    }
    if (operation == 1) {
        state->word_0c += value;
        return;
    }
    result = 0;
    if (state->word_0c == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord10(struct ScriptOperands *state, s32 operation, s32 value)
{
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

void Script_SetOrCompareHalfword20(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;
    u32 current;

    if (operation == 0) {
        state->halfword_20 = value;
        return;
    }
    if (operation == 1) {
        state->halfword_20 = (u16)((u32)state->halfword_20 + (u32)value);
        return;
    }
    current = state->halfword_20;
    asm volatile("" : "+l"(current)); /* FAKEMATCH: ⚓️ loads the halfword before extending the value */
    result = 0;
    if (current == (u32)(s16)value)
        result = 1;
    state->comparison_result = result;
}
