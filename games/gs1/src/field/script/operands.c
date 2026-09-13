#include "script_operands.h"

typedef void (*OperandFunc)(struct ScriptOperands *, s32, s32);
extern OperandFunc Data_080136e0[];

void Script_SetOrCompareAddress(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->script_address = value;
        return;
    }
    if (operation == 1) {
        state->script_address += (u32)value * 4;
        return;
    }
    result = 0;
    if (state->script_address == (u32)value) {
        result = 1;
    }
    state->comparison_result = result;
}

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

void Script_SetOrCompareUnsignedHalfword(struct ScriptOperands *state, s32 operation, s32 value)
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
  if (operation == 0)
  {
    state->halfword_20 = value;
    return;
  }
  if (operation == 1)
  {
    state->halfword_20 = (u16)((u32)state->halfword_20 + (u32)value);
    return;
  }
  result = 0;
  if (state->halfword_20 == (s16)value)
  {
    result = 1;
  }
  state->comparison_result = result;
}

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
        result = 0;
        if (state->comparison_result == (u8)value)
            result = 1;
        state->comparison_result = result;
    }
}

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

s32 Script_ApplyOperandSet(struct ScriptOperands *work)
{
    s16 index = (s16)work->cursor;
    u8 *entry = (u8 *)(work->script_address + index * 4 + 4);
    OperandFunc callback = Data_080136e0[*(s32 *)entry];

    if (callback != 0)
        callback(work, 0, *(s32 *)(entry + 4));
    work->cursor += 3;
    return 1;
}

s32 Script_ApplyOperandAdd(struct ScriptOperands *work)
{
    s16 index = (s16)work->cursor;
    u8 *entry = (u8 *)(work->script_address + index * 4 + 4);
    OperandFunc callback = Data_080136e0[*(s32 *)entry];

    if (callback != 0)
        callback(work, 1, *(s32 *)(entry + 4));
    work->cursor += 3;
    return 1;
}

s32 Script_ApplyOperandCompare(struct ScriptOperands *work)
{
    s16 index = (s16)work->cursor;
    u8 *entry = (u8 *)(work->script_address + index * 4 + 4);
    OperandFunc callback = Data_080136e0[*(s32 *)entry];

    if (callback != 0)
        callback(work, 2, *(s32 *)(entry + 4));
    work->cursor += 3;
    return 1;
}
