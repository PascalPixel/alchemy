/* Near miss of Func_020013d8, differ 4: the native loads x after the frame mask (movs r3, #15; ldr r0, [r7, #8]) and y after the offset is subtracted; here both loads come one step later. Operand barriers that tie the loads swap the object and parameter-block registers (r7 and r6). */
#include "../../../../games/tla/src/scene/apojii_islands/local.h"

s32 Func_020013d8(struct ObjectRuntime *object)
{
    s32 params[10];

    if (gFrameCount & 1) {
        params[1] = 7;
        if ((gFrameCount & 2) == 0)
            params[1] = 5;
        params[2] = 0xcccc;
        params[3] = 0xcccc;
        params[0] = 0;
        Func_0200015c(object->x + ((8 - (gFrameCount & 15)) << 16), object->y + 0x1a0000, object->z,
            0, -(((u32)(Random16Far() << 3) >> 16) * 0x3333), 0, 0xb0000, params);
    }
    return 0;
}
