#include "layout_guard.h"
#include "types.h"

typedef struct InterpolationPoint_08079b24 {
    s16 input;
    s16 output;
} InterpolationPoint_08079b24;

LAYOUT_SIZE_GUARD(
    InterpolationPoint08079b24_Size,
    InterpolationPoint_08079b24,
    4);

extern InterpolationPoint_08079b24 Data_08089258[];

s32 Func_080022ec(s32 dividend, s32 divisor);

/* Map a clamped value through the five-point descending lookup curve. */
s32 Func_08079b24(s32 value, s32 halve)
{
    const InterpolationPoint_08079b24 *points = Data_08089258;
    s32 clamped = value;
    s32 result;
    s32 index;

    if (clamped > points[0].input)
        clamped = points[0].input;
    else if (clamped < points[4].input)
        clamped = points[4].input;

    for (index = 1; index < 5; index++) {
        if (clamped > points[index].input)
            break;
    }

    if (index == 5) {
        result = points[4].output;
    } else {
        const InterpolationPoint_08079b24 *previous = &points[index - 1];
        const InterpolationPoint_08079b24 *current = &points[index];
        s32 numerator = previous->output - current->output;
        s32 denominator = previous->input - current->input;

        result = Func_080022ec(
            (clamped - current->input) * numerator,
            denominator);
        result += current->output;
    }

    if (halve == 1)
        result /= 2;

    return result + 0x100;
}
