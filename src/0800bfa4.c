#include "types.h"

struct Thing {
    u8 filler0[8];
    s32 field8;
    u8 filler12[4];
    s32 field16;
};

s32 Func_0800bfa4(struct Thing *thing, s32 *out)
{
    u8 *state = *(u8 **)0x03001e70;
    s32 *origin = (s32 *)(state + 228);
    s32 a;
    s32 b;
    s32 x;
    s32 y;

    a = origin[0] & 0xffff0000;
    b = origin[1] & 0xffff0000;
    x = thing->field8 - a;
    y = thing->field16 - b;
    if ((u32)(x + 0x001fffff) <= 0x012ffffe && y > 0 && y < 0xe00000) {
        *out++ = x >> 16;
        *out = y >> 16;
        return 0;
    }
    *out++ = 0;
    *out = 0;
    return -1;
}
