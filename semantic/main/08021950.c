#include "types.h"

void Func_08021950(void *first, void *second, u32 *output, s32 shift)
{
    s32 negativeShift = -shift * 4;
    s32 positiveShift = shift * 4;
    s32 outer = 0;

    do {
        u32 shifted = *(u32 *)second;
        u32 fallback;
        u32 result;
        s32 inner;

        second = (u8 *)second + 4;
        fallback = *(u32 *)first;
        result = 0;
        first = (u8 *)first + 4;
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
