#include "types.h"

typedef struct Obj {
    u8 pad0[8];
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} Obj;

Obj *Func_02006fd4(void);
Obj *Func_020070da(void);

s32 Func_02001980(void)
{
    Obj *a = Func_02006fd4();
    Obj *b = Func_020070da();
    s32 ax = a->x / 0x100000;
    s32 az = a->z / 0x100000;
    s32 bx = b->x / 0x100000;
    s32 bz = b->z / 0x100000;
    s32 dx = ax - bx;
    s32 dz;

    if (dx < 0) {
        dx = -dx;
    }
    dz = az - bz;
    if (dz < 0) {
        dz = -dz;
    }
    if (dx > 7 || dz > 5) {
        return 0;
    }
    return 1;
}
