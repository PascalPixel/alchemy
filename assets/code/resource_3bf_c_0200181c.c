#include "types.h"

typedef struct Obj {
    u8 pad0[8];
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} Obj;

Obj *Func_02006e70(void);
Obj *Func_02006e78(s32);

s32 Func_0200181c(void)
{
    Obj *a = Func_02006e70();
    Obj *b = Func_02006e78(0);
    s32 az = a->z / 0x100000;
    s32 ax = a->x / 0x100000;
    s32 bz = b->z / 0x100000;
    s32 bx = b->x / 0x100000;
    s32 d = az - bz;

    if (d >= -6 && d <= 6 && ax - 1 < bx && ax + 1 > bx) {
        return 1;
    }
    return 0;
}
