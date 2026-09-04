#include "types.h"
#include "global_cells.h"

#define Map_GetScreenRelativePosition Func_0800bfa4

struct Thing {
    u8 filler0[8];
    s32 field8;
    u8 filler12[4];
    s32 field16;
};

s32 Map_GetScreenRelativePosition(struct Thing *obj, s32 *out)
{
    u8 *state = *(u8 **)ADDR_03001E70;
    s32 *org = (s32 *)(state + 228);
    s32 a;
    s32 b;
    s32 x;
    s32 y;

    a = org[0] & 0xffff0000;
    b = org[1] & 0xffff0000;
    x = obj->field8 - a;
    y = obj->field16 - b;
    if ((u32)(x + 0x001fffff) <= 0x012ffffe && y > 0 && y < 0xe00000) {
        *out++ = x >> 16;
        *out = y >> 16;
        return 0;
    }
    *out++ = 0;
    *out = 0;
    return -1;
}
