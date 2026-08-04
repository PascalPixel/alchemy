#include "types.h"

typedef struct Obj {
    u8 pad0[8];
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} Obj;

Obj *Func_02006cb6(s32);

s32 Func_02001660(void)
{
    Obj *o = Func_02006cb6(0);
    s32 zz = o->z / 0x100000;
    s32 xx = o->x / 0x100000;

    if (xx > 45 && zz > 14 && xx <= 64 && zz <= 16) {
        return 0;
    }
    return 1;
}
