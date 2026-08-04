#include "types.h"
struct Object_020005e4 { u8 unknown_00[8]; s32 x; s32 y; s32 z; };
struct EffectParams_020005e4 { s32 unk00; s32 mode; s32 color1; s32 color2; u8 unknown_10[0x18]; };
s32 Func_020053b2();
s32 Func_020053d0();
s32 Func_020053e4();
s32 Func_020053f8();
s32 Func_020053f6();
void Func_0200078e();
s32 Func_020005e4(struct Object_020005e4 *object)
{
    struct EffectParams_020005e4 params;
    s32 phase, x, y, speed;
    phase = *(u32 *)0x03001e40 & 7;
    if (phase != 0) goto done;
    params.unk00 = 3 - (s32)((u32)(Func_020053b2() * 2) >> 16);
    params.color1 = 0x6666;
    params.color2 = 0x6666;
    params.mode = 14;
    x = object->x + (((s32)((u32)(Func_020053d0() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - (s32)((u32)(Func_020053e4() * 32) >> 16)) << 16);
    speed = Func_020053f6(((s32)((u32)(Func_020053f8() * 5) >> 16) << 16) + 0x00050000, 10);
    Func_0200078e(x, y, object->z, 0, speed, phase, 0x000b0000, &params);
done:
    return 0;
}
