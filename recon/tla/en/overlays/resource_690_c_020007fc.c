/* Near miss of Func_020007fc of overlay 690 (44 instructions, 2 differ), from ☀️'s
 * SceneEffect_UpdateOrbitPosition: the native reloads the angle between the shifts of the two
 * random values (`lsls r5; ldr r3; lsls r0`), the draft after both. Measured: a compound
 * assignment the same; the first shift as a forced temporary before a one-pass block gives
 * `ldr; lsrs r5; lsls r0` (4 differ), and with the operands swapped 8. */
#include "types.h"

s32 Math_Sine(s32 angle);
s32 Math_Cosine(s32 angle);
u32 Random16Far(void);

s32 Func_020007fc(s32 *p)
{
    s16 *q = (s16 *)p[20];
    s32 a, b;
    s32 d = Math_Sine(p[12]) * 2;

    if (d > 0)
        d = -d;
    p[2] = p[14] + Math_Cosine(p[12]) * 2;
    p[3] = p[15] + d;
    q[9] = Math_Cosine(p[12] + 0x8000) / 8;
    a = Random16Far();
    b = Random16Far();
    p[12] = p[12] + ((((u32)a << 9) >> 16) + (((u32)b << 9) >> 16)) + 0x400;
    return 0;
}
