#include "types.h"

void Graphics_MergeShiftedTileRows(u32 *first, u32 *second, u32 *output, s32 shift)
{
    s32 neg;
    s32 rshift;
    s32 lshift;
    s32 i;

    neg = -shift;
    i = 0;
    rshift = neg * 4;
    lshift = shift * 4;

    do {
        u32 result;
        u32 shifted;
        u32 fallback;
        s32 j;

        shifted = *second++;
        result = 0;
        fallback = *first++;
        if (shift < 0)
            shifted >>= rshift;
        else
            shifted <<= lshift;

        j = 7;
        do {
            result <<= 4;
            if (shifted > 0x0FFFFFFF)
                result += shifted >> 28;
            else
                result += fallback >> 28;
            j--;
            shifted <<= 4;
            fallback <<= 4;
        } while (j >= 0);

        *output++ = result;
        i++;
    } while (i <= 7);
}
