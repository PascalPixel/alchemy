/* Near miss: the right instructions in other registers. The native keeps the first sine in r8 across
 * the two later calls, scales it there (mov r8, r0; mov r3, r8; lsls r3, r3, #4; asrs r3, r3, #15;
 * mov r8, r3) and copies it to r7 only when it adds the 16 (mov r7, r8; adds r7, #16); the second and
 * third are scaled after the last call, in place (r6 and r0). The fourth parameter is in r10, the
 * counter in r4 and the output pointer in r5. This draft puts the parameter in r8 and the first level
 * in r10. Tried: all three scaled in their call statements, the 16 added inside the loop expression. */
#include "types.h"
#include "iwram_call.h"

s32 Trig_Sin(s32 angle);

void Func_081496c8(s32 phase, s32 red_angle, s32 green_angle, s32 blue_angle)
{
    u16 colors[64];
    s32 red;
    s32 green;
    s32 blue;
    s32 i;

    phase <<= 10;
    red = Trig_Sin(phase + red_angle);
    red = (red << 4) >> 15;
    green = Trig_Sin(phase + green_angle);
    blue = Trig_Sin(phase + blue_angle);
    colors[0] = 0;
    green = (green << 4) >> 15;
    blue = (blue << 4) >> 15;
    red += 16;
    green += 16;
    blue += 16;
    for (i = 1; i < 64; i++) {
        s32 r = i * red / 64;
        s32 g = i * green / 64;
        s32 b = i * blue / 64;

        if (r < 0)
            r = 0;
        if (r > 31)
            r = 31;
        if (g < 0)
            g = 0;
        if (g > 31)
            g = 31;
        if (b < 0)
            b = 0;
        if (b > 31)
            b = 31;
        colors[i] = (b << 10) | (g << 5) | r;
    }
    Iwram_CopyWords((void *)0x05000002, colors, 128);
}
