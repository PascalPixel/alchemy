#include "curve.h"

s32 Curve_GetFirstSampleA(const s8 *samples)
{
    return samples[0] << CURVE_VALUE_SHIFT;
}

s32 Curve_LerpTwoSamples(const s8 *samples, s32 position)
{
    s32 start;
    s32 delta;

    start = samples[0] << CURVE_VALUE_SHIFT;
    delta = ((samples[1] << CURVE_VALUE_SHIFT) - start) * position;
    if (delta < 0)
    {
        delta += CURVE_FULL_STEPS - 1;
    }
    return start + (delta >> 4);
}

s32 Curve_LerpTwoSamplesB(const s8 *samples, s32 unused, s32 position)
{
    s32 base;
    s32 offset;

    base = samples[0] << CURVE_VALUE_SHIFT;
    offset = ((samples[1] << CURVE_VALUE_SHIFT) - base) * position;
    if (offset < 0)
    {
        offset += CURVE_FULL_STEPS - 1;
    }
    return base + (offset >> 4);
}

s32 Curve_StepAtSummedPosition(const s8 *samples, s32 start, s32 end)
{
    s32 first;
    s32 second;
    s32 maximum;
    u32 position;

    first = samples[0] << CURVE_VALUE_SHIFT;
    second = samples[1] << CURVE_VALUE_SHIFT;
    maximum = first;
    if (second > first)
    {
        maximum = second;
    }
    position = start + end;
    if (position == CURVE_FULL_STEPS - 1)
    {
        return maximum;
    }
    if (position < CURVE_FULL_STEPS - 1)
    {
        second = first;
    }
    return second;
}

s32 Curve_StepAtDifferencePosition(const s8 *samples, s32 start, s32 end)
{
    s32 first;
    s32 second;
    s32 maximum;
    s32 difference;
    u32 position = start;

    first = samples[0] << CURVE_VALUE_SHIFT;
    second = samples[1] << CURVE_VALUE_SHIFT;
    maximum = first;
    if (second > first)
    {
        maximum = second;
    }
    difference = end - position;
    position = difference + CURVE_FULL_STEPS - 1;
    if (position == CURVE_FULL_STEPS - 1)
    {
        return maximum;
    }
    if (position < CURVE_FULL_STEPS - 1)
    {
        second = first;
    }
    return second;
}

s32 Curve_LerpThreeSamplesByRatio(const s8 *samples, s32 start, s32 end)
{
    s32 first;
    s32 middle;
    s32 last;
    s32 result;

    first = *samples++ << CURVE_VALUE_SHIFT;
    middle = *samples << CURVE_VALUE_SHIFT;
    last = samples[1] << CURVE_VALUE_SHIFT;
    start = start + end;
    if ((u32)start == CURVE_FULL_STEPS - 1)
    {
        result = middle;
    }
    else if ((u32)start < CURVE_FULL_STEPS - 1)
    {
        result = first + (middle - first) * start / (CURVE_FULL_STEPS - 1);
    }
    else
    {
        start = start - (CURVE_FULL_STEPS - 1);
        result = middle + (last - middle) * start / (CURVE_FULL_STEPS - 1);
    }
    return result;
}
