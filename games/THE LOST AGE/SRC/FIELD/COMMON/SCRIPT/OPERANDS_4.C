#include "SCRIPT_OPERANDS.H"

typedef void (*OperandFunc)(struct ScriptOperands *, s32, s32);
extern OperandFunc Data_0802f2dc[];

void Script_SetOrCompareByte58(struct ScriptOperands *state, s32 operation, s32 value)
{
    s32 result;
    if (operation == 0) {
        state->byte_58 = value;
    } else if (operation == 1) {
        state->byte_58 = (u8)((u32)state->byte_58 + (u32)value);
    } else {
        result = 0;
        if (state->byte_58 == (u8)value)
            result = 1;
        state->comparison_result = result;
    }
}

void Script_SetOrCompareByte59(struct ScriptOperands *state, s32 operation, s32 value)
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

void Script_SetOrCompareByte5a(struct ScriptOperands *state, s32 operation, s32 value)
{
    s32 result;
    if (operation == 0) {
        state->byte_5a = value;
    } else if (operation == 1) {
        state->byte_5a = (u8)((u32)state->byte_5a + (u32)value);
    } else {
        result = 0;
        if (state->byte_5a == (u8)value)
            result = 1;
        state->comparison_result = result;
    }
}

void Script_SetOrCompareByte5b(struct ScriptOperands *state, s32 operation, s32 value)
{
    s32 result;
    if (operation == 0) {
        state->byte_5b = value;
    } else if (operation == 1) {
        state->byte_5b = (u8)((u32)state->byte_5b + (u32)value);
    } else {
        result = 0;
        if (state->byte_5b == (u8)value)
            result = 1;
        state->comparison_result = result;
    }
}

void Script_SetOrCompareByte5d(struct ScriptOperands *work, s32 operation, s32 value)
{
    s32 result;

    if (operation == 0) {
        work->byte_5d = value;
    } else if (operation == 1) {
        work->byte_5d += value;
    } else {
        result = 0;
        if (work->byte_5d == (u8)value)
            result = 1;
        work->comparison_result = result;
    }
}

void Script_SetOrCompareHalfword5e(struct ScriptOperands *work, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        work->halfword_5e = value;
        return;
    }
    if (operation == 1) {
        work->halfword_5e = work->halfword_5e + value;
        return;
    }
    result = 0;
    if ((s16)work->halfword_5e == (s16)value) {
        result = 1;
    }
    work->comparison_result = result;
}

void Script_SetOrCompareSignedHalfword64(struct ScriptOperands *work, s32 operation, s32 value)
{
    s32 result;

    if (operation == 0) {
        work->signed_halfword_64 = value;
    } else if (operation == 1) {
        work->signed_halfword_64 =
            (u16)work->signed_halfword_64 + value;
    } else {
        result = 0;
        if (work->signed_halfword_64 == (s16)value)
            result = 1;
        work->comparison_result = result;
    }
}

void Script_SetOrCompareSignedHalfword66(struct ScriptOperands *work, s32 operation, s32 value)
{
    s32 result;

    if (operation == 0) {
        work->signed_halfword_66 = value;
    } else if (operation == 1) {
        work->signed_halfword_66 =
            (u16)work->signed_halfword_66 + value;
    } else {
        result = 0;
        if (work->signed_halfword_66 == (s16)value)
            result = 1;
        work->comparison_result = result;
    }
}

void Script_SetOrCompareWord68(struct ScriptOperands *work, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        work->word_68 = value;
        return;
    }
    if (operation == 1) {
        work->word_68 = work->word_68 + (u32)value;
        return;
    }
    result = 0;
    if (work->word_68 == (u32)value) {
        result = 1;
    }
    work->comparison_result = result;
}

void Script_SetOrCompareWord6c(struct ScriptOperands *work, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        work->word_6c = value;
        return;
    }
    if (operation == 1) {
        work->word_6c = work->word_6c + (u32)value;
        return;
    }
    result = 0;
    if (work->word_6c == (u32)value) {
        result = 1;
    }
    work->comparison_result = result;
}

void Script_SetOrCompareByte62(struct ScriptOperands *work, s32 operation, s32 value)
{
    s32 result;
    if (operation == 0) {
        work->byte_62 = value;
    } else if (operation == 1) {
        work->byte_62 += value;
    } else {
        result = 0;
        if (work->byte_62 == (u8)value)
            result = 1;
        work->comparison_result = result;
    }
}

void Script_SetOrCompareByte63(struct ScriptOperands *work, s32 operation, s32 value)
{
    s32 result;
    if (operation == 0) {
        work->byte_63 = value;
    } else if (operation == 1) {
        work->byte_63 += value;
    } else {
        result = 0;
        if (work->byte_63 == (u8)value)
            result = 1;
        work->comparison_result = result;
    }
}
