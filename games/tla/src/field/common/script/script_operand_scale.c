#include "script.h"

void Script_SetOrCompareScaleX(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0)
    {
        state->word18 = value;
        return;
    }
    if (operation == 1)
    {
        state->word18 += value;
        return;
    }
    result = 0;
    if (state->word18 == value)
    {
        result = 1;
    }
    state->comparisonResult = result;
}

void Script_SetOrCompareScaleY(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0)
    {
        state->word1c = value;
        return;
    }
    if (operation == 1)
    {
        state->word1c += value;
        return;
    }
    result = 0;
    if (state->word1c == value)
    {
        result = 1;
    }
    state->comparisonResult = result;
}

void Script_SetOrCompareVelocityX(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0)
    {
        state->word24 = value;
        return;
    }
    if (operation == 1)
    {
        state->word24 += value;
        return;
    }
    result = 0;
    if (state->word24 == value)
    {
        result = 1;
    }
    state->comparisonResult = result;
}

void Script_SetOrCompareVelocityY(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0)
    {
        state->word28 = value;
        return;
    }
    if (operation == 1)
    {
        state->word28 += value;
        return;
    }
    result = 0;
    if (state->word28 == value)
    {
        result = 1;
    }
    state->comparisonResult = result;
}

void Script_SetOrCompareVelocityZ(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0)
    {
        state->word2c = value;
        return;
    }
    if (operation == 1)
    {
        state->word2c += value;
        return;
    }
    result = 0;
    if (state->word2c == value)
    {
        result = 1;
    }
    state->comparisonResult = result;
}

void Script_SetOrCompareSpeedLimit(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0)
    {
        state->word30 = value;
        return;
    }
    if (operation == 1)
    {
        state->word30 += value;
        return;
    }
    result = 0;
    if (state->word30 == value)
    {
        result = 1;
    }
    state->comparisonResult = result;
}

void Script_SetOrCompareAcceleration(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0)
    {
        state->word34 = value;
        return;
    }
    if (operation == 1)
    {
        state->word34 += value;
        return;
    }
    result = 0;
    if (state->word34 == value)
    {
        result = 1;
    }
    state->comparisonResult = result;
}

void Script_SetOrCompareTargetX(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0)
    {
        state->word38 = value;
        return;
    }
    if (operation == 1)
    {
        state->word38 += value;
        return;
    }
    result = 0;
    if (state->word38 == value)
    {
        result = 1;
    }
    state->comparisonResult = result;
}

void Script_SetOrCompareTargetY(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0)
    {
        state->word3c = value;
        return;
    }
    if (operation == 1)
    {
        state->word3c += value;
        return;
    }
    result = 0;
    if (state->word3c == value)
    {
        result = 1;
    }
    state->comparisonResult = result;
}

void Script_SetOrCompareTargetZ(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0)
    {
        state->word40 = value;
        return;
    }
    if (operation == 1)
    {
        state->word40 += value;
        return;
    }
    result = 0;
    if (state->word40 == value)
    {
        result = 1;
    }
    state->comparisonResult = result;
}

void Script_SetOrCompareVerticalMotion(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0)
    {
        state->word44 = value;
        return;
    }
    if (operation == 1)
    {
        state->word44 += value;
        return;
    }
    result = 0;
    if (state->word44 == value)
    {
        result = 1;
    }
    state->comparisonResult = result;
}

void Script_SetOrCompareGravity(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0)
    {
        state->word48 = value;
        return;
    }
    if (operation == 1)
    {
        state->word48 += value;
        return;
    }
    result = 0;
    if (state->word48 == value)
    {
        result = 1;
    }
    state->comparisonResult = result;
}

void Script_SetOrCompareTerrainHeight(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0)
    {
        state->word14 = value;
        return;
    }
    if (operation == 1)
    {
        state->word14 += value;
        return;
    }
    result = 0;
    if (state->word14 == value)
    {
        result = 1;
    }
    state->comparisonResult = result;
}

void Script_SetOrCompareWord4c(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0)
    {
        state->word4c = value;
        return;
    }
    if (operation == 1)
    {
        state->word4c += value;
        return;
    }
    result = 0;
    if (state->word4c == value)
    {
        result = 1;
    }
    state->comparisonResult = result;
}

void Script_SetOrCompareAddress50(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0)
    {
        state->address50 = value;
        return;
    }
    if (operation == 1)
    {
        state->address50 += value * 4;
        return;
    }
    result = 0;
    if (state->address50 == value)
    {
        result = 1;
    }
    state->comparisonResult = result;
}

void Script_SetOrCompareByte54(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == 0)
    {
        state->byte54 = value;
    }
    else if (operation == 1)
    {
        state->byte54 += value;
    }
    else
    {
        state->comparisonResult = state->byte54 == (u8)value;
    }
}

void Script_SetOrCompareMotionFlags(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == 0)
    {
        state->byte55 = value;
    }
    else if (operation == 1)
    {
        state->byte55 += value;
    }
    else
    {
        state->comparisonResult = state->byte55 == (u8)value;
    }
}

void Script_SetOrCompareArrivalAxis(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == 0)
    {
        state->byte56 = value;
    }
    else if (operation == 1)
    {
        state->byte56 += value;
    }
    else
    {
        state->comparisonResult = state->byte56 == (u8)value;
    }
}

void Script_SetOrCompareComparisonResult(struct ScriptOperands *state, s32 operation, s32 value)
{
    s32 result;

    if (operation == 0)
    {
        state->comparisonResult = value;
    }
    else if (operation == 1)
    {
        state->comparisonResult = (u8)((u32)state->comparisonResult + (u32)value);
    }
    else
    {
        u8 current = state->comparisonResult;

        /* FAKEMATCH: ⚓️ loads the byte before narrowing the value */
        asm volatile("" : "+l"(current));
        result = 0;
        if (current == (u8)value)
            result = 1;
        state->comparisonResult = result;
    }
}
