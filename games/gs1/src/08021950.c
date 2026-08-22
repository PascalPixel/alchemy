#include "types.h"

void Func_08021950(u32 *first, u32 *second, u32 *output, s32 shift)
{
    s32 negatedShift;
    s32 negativeShift;
    s32 positiveShift;
    s32 outer;

    negatedShift = -shift;
    outer = 0;
    negativeShift = negatedShift * 4;
    positiveShift = shift * 4;

    do {
        u32 result;
        u32 shifted;
        u32 fallback;
        s32 inner;

        shifted = *second++;
        result = 0;
        fallback = *first++;
        if (shift < 0)
            shifted >>= negativeShift;
        else
            shifted <<= positiveShift;

        inner = 7;
        do {
            result <<= 4;
            if (shifted > 0x0FFFFFFF)
                result += shifted >> 28;
            else
                result += fallback >> 28;
            inner--;
            shifted <<= 4;
            fallback <<= 4;
        } while (inner >= 0);

        *output++ = result;
        outer++;
    } while (outer <= 7);
}
