#include "types.h"

extern volatile s32 Data_03001e40;
extern s32 Func_03000380(s32 dividend, s32 divisor);
extern s32 Func_080000f8(void);
extern void Func_0200013c(s32 x, s32 y, s32 z, s32 kind,
                          s32 arg4, s32 arg5, u32 flags, void *descriptor);

/* Complete 172-byte actor-relative particle owner through its three-word pool. */
s32 Func_020022d8(u8 *actor)
{
    u8 descriptor[40];
    u32 phase = (u32)Data_03001e40 & 7;
    u32 draw;
    s32 x;
    s32 y;
    s32 z;
    s32 scale;

    if (phase != 0)
        return 0;

    *(s32 *)(descriptor + 4) = 7;
    *(s32 *)(descriptor + 8) = 0xb333;
    *(s32 *)(descriptor + 12) = 0xb333;

    draw = (u32)Func_080000f8();
    x = *(s32 *)(actor + 8) + ((s32)((draw * 17) >> 16) - 8) * 0x10000;
    draw = (u32)Func_080000f8();
    y = *(s32 *)(actor + 12) + (s32)((draw * 17) >> 16) * 0x10000;
    draw = (u32)Func_080000f8();
    z = *(s32 *)(actor + 16) + ((s32)((draw * 17) >> 16) - 8) * 0x10000;
    draw = (u32)Func_080000f8();
    scale = Func_03000380((s32)((draw * 5) >> 16) * 0x10000 + 0x30000, 10);

    Func_0200013c(x, y, z, 0, scale, (s32)phase, 0x00090001,
                  descriptor);
    return 0;
}
