#include "SCRIPT_OPERANDS.H"

void Script_SetOrCompareWord18(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_18 = value;
        return;
    }
    if (operation == 1) {
        state->word_18 += value;
        return;
    }
    result = 0;
    if (state->word_18 == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord1c(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_1c = value;
        return;
    }
    if (operation == 1) {
        state->word_1c += value;
        return;
    }
    result = 0;
    if (state->word_1c == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord24(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_24 = value;
        return;
    }
    if (operation == 1) {
        state->word_24 += value;
        return;
    }
    result = 0;
    if (state->word_24 == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord28(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_28 = value;
        return;
    }
    if (operation == 1) {
        state->word_28 += value;
        return;
    }
    result = 0;
    if (state->word_28 == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord2c(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_2c = value;
        return;
    }
    if (operation == 1) {
        state->word_2c += value;
        return;
    }
    result = 0;
    if (state->word_2c == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord30(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_30 = value;
        return;
    }
    if (operation == 1) {
        state->word_30 += value;
        return;
    }
    result = 0;
    if (state->word_30 == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord34(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_34 = value;
        return;
    }
    if (operation == 1) {
        state->word_34 += value;
        return;
    }
    result = 0;
    if (state->word_34 == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord38(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_38 = value;
        return;
    }
    if (operation == 1) {
        state->word_38 += value;
        return;
    }
    result = 0;
    if (state->word_38 == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord3c(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_3c = value;
        return;
    }
    if (operation == 1) {
        state->word_3c += value;
        return;
    }
    result = 0;
    if (state->word_3c == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord40(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_40 = value;
        return;
    }
    if (operation == 1) {
        state->word_40 += value;
        return;
    }
    result = 0;
    if (state->word_40 == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord44(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_44 = value;
        return;
    }
    if (operation == 1) {
        state->word_44 += value;
        return;
    }
    result = 0;
    if (state->word_44 == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord48(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_48 = value;
        return;
    }
    if (operation == 1) {
        state->word_48 += value;
        return;
    }
    result = 0;
    if (state->word_48 == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord14(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_14 = value;
        return;
    }
    if (operation == 1) {
        state->word_14 += value;
        return;
    }
    result = 0;
    if (state->word_14 == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord4c(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_4c = value;
        return;
    }
    if (operation == 1) {
        state->word_4c += value;
        return;
    }
    result = 0;
    if (state->word_4c == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareAddress50(struct ScriptOperands *state, s32 operation, s32 value)
{
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

void Script_SetOrCompareByte54(struct ScriptOperands *state, s32 operation, s32 value)
{
    s32 result;

    if (operation == 0) {
        state->byte_54 = value;
    } else if (operation == 1) {
        state->byte_54 = (u8)((u32)state->byte_54 + (u32)value);
    } else {
        result = 0;
        if (state->byte_54 == (u8)value)
            result = 1;
        state->comparison_result = result;
    }
}

void Script_SetOrCompareByte55(struct ScriptOperands *state, s32 operation, s32 value)
{
    s32 result;
    if (operation == 0) {
        state->byte_55 = value;
    } else if (operation == 1) {
        state->byte_55 = (u8)((u32)state->byte_55 + (u32)value);
    } else {
        result = 0;
        if (state->byte_55 == (u8)value)
            result = 1;
        state->comparison_result = result;
    }
}

void Script_SetOrCompareByte56(struct ScriptOperands *state, s32 operation, s32 value)
{
    s32 result;
    if (operation == 0) {
        state->byte_56 = value;
    } else if (operation == 1) {
        state->byte_56 = (u8)((u32)state->byte_56 + (u32)value);
    } else {
        result = 0;
        if (state->byte_56 == (u8)value)
            result = 1;
        state->comparison_result = result;
    }
}

void Script_SetOrCompareComparisonResult(struct ScriptOperands *state, s32 operation, s32 value)
{
    s32 result;

    if (operation == 0) {
        state->comparison_result = value;
    } else if (operation == 1) {
        state->comparison_result =
            (u8)((u32)state->comparison_result + (u32)value);
    } else {
        u8 current = state->comparison_result;

        asm volatile("" : "+l"(current)); /* FAKEMATCH: ⚓️ loads the byte before narrowing the value */
        result = 0;
        if (current == (u8)value)
            result = 1;
        state->comparison_result = result;
    }
}
