#include "script.h"
#include "types.h"

typedef void (*OperandFunc)(struct ScriptOperands *, s32, s32);
extern OperandFunc Script_OperandHandlerTable[];

void Script_SetOrCompareSnapToTarget(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == 0) {
        state->byte_58 = value;
    } else if (operation == 1) {
        state->byte_58 += value;
    } else {
        state->comparison_result = state->byte_58 == (u8)value;
    }
}

void Script_SetOrCompareCollisionFlags(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == 0) {
        state->byte_59 = value;
    } else if (operation == 1) {
        state->byte_59 += value;
    } else {
        state->comparison_result = state->byte_59 == (u8)value;
    }
}

void Script_SetOrCompareSteeringFlags(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == 0) {
        state->byte_5a = value;
    } else if (operation == 1) {
        state->byte_5a += value;
    } else {
        state->comparison_result = state->byte_5a == (u8)value;
    }
}

void Script_SetOrComparePaused(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == 0) {
        state->byte_5b = value;
    } else if (operation == 1) {
        state->byte_5b += value;
    } else {
        state->comparison_result = state->byte_5b == (u8)value;
    }
}

void Script_SetOrCompareRepeatCount(struct ScriptOperands *work, s32 operation, s32 value)
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
        work->halfword_5e += value;
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
    if (operation == 0) {
        work->signed_halfword_64 = value;
    } else if (operation == 1) {
        work->signed_halfword_64 += value;
    } else {
        work->comparison_result = work->signed_halfword_64 == (s16)value;
    }
}

void Script_SetOrCompareSignedHalfword66(struct ScriptOperands *work, s32 operation, s32 value)
{
    if (operation == 0) {
        work->signed_halfword_66 = value;
    } else if (operation == 1) {
        work->signed_halfword_66 += value;
    } else {
        work->comparison_result = work->signed_halfword_66 == (s16)value;
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
        work->word_68 += value;
        return;
    }
    result = 0;
    if (work->word_68 == value) {
        result = 1;
    }
    work->comparison_result = result;
}

void Script_SetOrCompareUpdateAddress(struct ScriptOperands *work, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        work->word_6c = value;
        return;
    }
    if (operation == 1) {
        work->word_6c += value;
        return;
    }
    result = 0;
    if (work->word_6c == value) {
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
    s16 index = work->cursor;
    s32 *entry = (s32 *)work->script_address + index + 1;
    OperandFunc callback = Script_OperandHandlerTable[entry[0]];

    if (callback != 0)
        callback(work, 0, entry[1]);
    work->cursor += 3;
    return 1;
}

s32 Script_ApplyOperandAdd(struct ScriptOperands *work)
{
    s16 index = work->cursor;
    s32 *entry = (s32 *)work->script_address + index + 1;
    OperandFunc callback = Script_OperandHandlerTable[entry[0]];

    if (callback != 0)
        callback(work, 1, entry[1]);
    work->cursor += 3;
    return 1;
}

s32 Script_ApplyOperandCompare(struct ScriptOperands *work)
{
    s16 index = work->cursor;
    s32 *entry = (s32 *)work->script_address + index + 1;
    OperandFunc callback = Script_OperandHandlerTable[entry[0]];

    if (callback != 0)
        callback(work, 2, entry[1]);
    work->cursor += 3;
    return 1;
}

struct ScriptObject {
    u8 unknown_00[0x6c];
    s32 value;
};

void ScriptObject_SetValue(struct ScriptObject *object, s32 value)
{
    if (object != NULL)
        object->value = value;
}
