#include "types.h"

extern volatile s32 Data_03001e40;




/* Complete 172-byte actor-relative particle owner through its three-word pool. */
extern s32 Func_02004d4c(void);
extern s32 Func_02004d60(void);
extern s32 Func_02004d70(void);
extern s32 Func_02004d82(void);
extern s32 Func_02004d78(s32 dividend, s32 divisor);
extern void Func_020024a0(s32 x, s32 y, s32 z, s32 kind,
                          s32 arg4, s32 arg5, u32 flags, void *descriptor);
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

    x = *(s32 *)(actor + 8) + ((s32)((draw * 17) >> 16) - 8) * 0x10000;
    draw = (u32)Func_02004d4c();
    draw = (u32)Func_02004d60();
    y = *(s32 *)(actor + 12) + (s32)((draw * 17) >> 16) * 0x10000;
    draw = (u32)Func_02004d70();
    z = *(s32 *)(actor + 16) + ((s32)((draw * 17) >> 16) - 8) * 0x10000;
    draw = (u32)Func_02004d82();
    scale = Func_02004d78((s32)((draw * 5) >> 16) * 0x10000 + 0x30000, 10);

    Func_020024a0(x, y, z, 0, scale, (s32)phase, 0x00090001,
                  descriptor);
    return 0;
}
