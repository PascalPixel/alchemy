#include "types.h"

typedef struct Obj {
    u8 pad0[8];
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} Obj;

extern u8 *Data_03001ebc;

Obj *Func_0200703e(s32);
s32 Func_02007006(s32);
void Func_02002960(void);

void Func_020019e8(void)
{
    Obj *o = Func_0200703e(0);

    if (Func_02007006(859) == 0) {
        s32 ax = o->x / 0x100000;
        s32 az = o->z / 0x100000;

        if (ax == 43 && az > 28 && az <= 31) {
            s16 *q = (s16 *)(Data_03001ebc + 364);
            s32 v = 41;

            *q = v;
            Func_02002960();
        }
    }
}
