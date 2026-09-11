#include "types.h"
#include "scene.h"

/* graphics/color/reserved_no_op.c */
void Graphics_NoOpColor(void)
{
}

/* graphics/color/interpolate_palette_buffers.c */
void Graphics_InterpolatePaletteBuffers(s16 *a, s16 *b, s16 *dst, s32 n)
{
    s32 index;
    s32 first;
    s32 second;
    s32 (*divide)(s32, s32);

    if (n > 0) {
        divide = (s32 (*)(s32, s32))0x03000380;
        index = 0x5FF;
        do {
            first = *a;
            second = *b;
            *dst = divide(second - first, n);
            index--;
            a++;
            b++;
            dst++;
        } while (index >= 0);
    }
}
