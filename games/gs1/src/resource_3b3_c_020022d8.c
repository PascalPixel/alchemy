#include "types.h"

extern volatile s32 Data_03001e40;

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
    u8 *d;
    u32 phase = (u32)Data_03001e40 & 7;
    s32 x;
    s32 y;
    s32 z;
    s32 scale;

    if (phase != 0)
        return 0;

    d = descriptor;
    *(s32 *)(d + 4) = 7;
    *(s32 *)(d + 8) = 0xb333;
    *(s32 *)(d + 12) = 0xb333;

    x = *(s32 *)(actor + 8) + (((s32)(((u32)Func_02004d4c() * 17) >> 16) - 8) << 16);
    y = *(s32 *)(actor + 12) + ((s32)(((u32)Func_02004d60() * 17) >> 16) << 16);
    z = *(s32 *)(actor + 16) + (((s32)(((u32)Func_02004d70() * 17) >> 16) - 8) << 16);
    scale = Func_02004d78((s32)(((u32)Func_02004d82() * 5) >> 16) * 0x10000 + 0x30000, 10);

    Func_020024a0(x, y, z, 0, scale, (s32)phase, 0x00090001, d);
    return 0;
}
