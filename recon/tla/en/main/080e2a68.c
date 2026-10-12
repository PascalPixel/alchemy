/* Near miss of Func_080e2a68 (48 instructions, 4 differ): the native starts i / 2 (lsrs r2, r7, #31)
 * after the row sum and finishes it after the offset load; the draft starts it one row early. */
#include "types.h"

s32 Trig_Cos(s32 angle);
s32 Trig_Sin(s32 angle);

void Func_080e2a68(s32 radius, s32 angle, u16 *offsets, u8 *pixels)
{
    s32 i;

    if (radius > 31)
        radius = 31;
    for (i = 0; i <= 29; i++) {
        s32 x = ((radius * Trig_Cos(angle)) >> 16) + 32;
        s32 y = ((radius * Trig_Sin(angle)) >> 16) + 32;
        s32 value = i / 2 + 225;

        pixels[offsets[y * 64 + x]] = value;
        angle += 0x170;
    }
}
