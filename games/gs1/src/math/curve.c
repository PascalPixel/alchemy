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
    if (delta < 0) {
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
    if (offset < 0) {
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
    if (second > first) {
        maximum = second;
    }
    position = start + end;
    if (position == CURVE_FULL_STEPS - 1) {
        return maximum;
    }
    if (position < CURVE_FULL_STEPS - 1) {
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
    if (second > first) {
        maximum = second;
    }
    difference = end - position;
    position = difference + CURVE_FULL_STEPS - 1;
    if (position == CURVE_FULL_STEPS - 1) {
        return maximum;
    }
    if (position < CURVE_FULL_STEPS - 1) {
        second = first;
    }
    return second;
}

s32 FixedPoint_Ratio(s32 dividend, s32 divisor);

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
    if ((u32)start == CURVE_FULL_STEPS - 1) {
        result = middle;
    } else if ((u32)start < CURVE_FULL_STEPS - 1) {
        result = first + FixedPoint_Ratio(
            (middle - first) * start,
            CURVE_FULL_STEPS - 1);
    } else {
        start = start - (CURVE_FULL_STEPS - 1);
        result = middle + FixedPoint_Ratio(
            (last - middle) * start,
            CURVE_FULL_STEPS - 1);
    }
    return result;
}

s32 Curve_LerpThreeSamplesAtDifference(const s8 *samples, s32 start, s32 end)
{
    s32 first;
    s32 second;
    s32 third;
    s32 difference;
    s32 delta;

    first = *samples++ << CURVE_VALUE_SHIFT;
    second = samples[0] << CURVE_VALUE_SHIFT;
    third = samples[1];
    difference = end - start;
    start = difference + CURVE_FULL_STEPS - 1;
    third <<= CURVE_VALUE_SHIFT;

    if (start == CURVE_FULL_STEPS - 1)
        return second;

    if ((u32)start < CURVE_FULL_STEPS - 1) {
        delta = second - first;
        difference = delta *start;
        if (difference < 0)
            difference += CURVE_FULL_STEPS - 1;
        difference >>= 4;
        return first + difference;
    } else {
        difference *= third - second;
        if (difference < 0)
            difference += CURVE_FULL_STEPS - 1;
        difference >>= 4;
        return second + difference;
    }
}

s32 Curve_LerpTwoSamplesByTable(const s8 *samples, s32 position, s32 row)
{
    s32 start;

    start = samples[0] << CURVE_VALUE_SHIFT;
    return start
        + (((samples[1] << CURVE_VALUE_SHIFT) - start)
           * Data_080132fc[position + (row *CURVE_FULL_STEPS)]);
}

s32 Curve_StepAtHalf(const s8 *samples, u32 position)
{
    s32 value;
    if (position < CURVE_HALF_STEPS)
        value = samples[0];
    else
        value = samples[1];
    return value << CURVE_VALUE_SHIFT;
}

s32 Curve_StepAtHalfB(const s8 *samples, u32 unused, u32 position)
{
    s32 value;
    if (position < CURVE_HALF_STEPS)
        value = samples[0];
    else
        value = samples[1];
    return value << CURVE_VALUE_SHIFT;
}

s32 Curve_LerpThreeSamplesAtHalf(s8 *samples, u32 position)
{
    s32 a;
    s32 b;
    s32 delta;

    a = *samples++ << CURVE_VALUE_SHIFT;
    b = *samples++ << CURVE_VALUE_SHIFT;
    if (position < CURVE_HALF_STEPS) {
        delta = (b - a) * position;
        if (delta < 0) {
            delta += CURVE_HALF_STEPS - 1;
        }
        return a + (delta >> 3);
    }

    {
        s32 pos;

        a = *samples << CURVE_VALUE_SHIFT;
        pos = position;
        a -= b;
        pos -= CURVE_HALF_STEPS;
        delta = a *pos;
        if (delta < 0) {
            delta += CURVE_HALF_STEPS - 1;
        }
        return b + (delta >> 3);
    }
}

s32 Curve_LerpThreeSamplesAtHalfB(u8 *samples, u32 unused, u32 position)
{
    s32 a;
    s32 b;
    s32 pos;
    s32 delta;
    s32 tail;

    a = *samples++ << CURVE_VALUE_SHIFT;
    b = *samples++ << CURVE_VALUE_SHIFT;
    if (position < CURVE_HALF_STEPS) {
        delta = (b - a) * position;
        if (delta < 0) {
            delta += CURVE_HALF_STEPS - 1;
        }
        return a + (delta >> 3);
    }

    a = *samples << CURVE_VALUE_SHIFT;
    pos = position;
    a -= b;
    pos -= CURVE_HALF_STEPS;
    tail = a *pos;
    if (tail < 0) {
        tail += CURVE_HALF_STEPS - 1;
    }
    return b + (tail >> 3);
}

s32 Curve_LookupSampleByTable(const s8 *samples, u32 position, u32 row)
{
    return samples[Data_080133fc[position + (row << 4)]]
        << CURVE_VALUE_SHIFT;
}

s32 Curve_LookupSampleByTableReversed(const s8 *samples, u32 position, u32 row)
{
    return samples[
        Data_080133fc[((row << 4) - position) + CURVE_FULL_STEPS - 1]]
        << CURVE_VALUE_SHIFT;
}

s32 Curve_GetFirstSampleB(const s8 *samples)
{
    return samples[0] << CURVE_VALUE_SHIFT;
}

s32 Curve_GetFirstSampleC(const s8 *samples)
{
    return samples[0] << CURVE_VALUE_SHIFT;
}
