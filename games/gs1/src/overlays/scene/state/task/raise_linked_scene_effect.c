#include "types.h"

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[65];
    u8 f55;
    u8 f56[14];
    s16 f64;
} Obj;

extern u8 Data_0200c288[];

extern Obj *Func_02007254(s16);
extern void Func_0200718e(Obj *, s32, s32, s32);
extern void Func_02007176(Obj *, s32);
extern void Func_020073dc(s32);

s32 Func_020032f0(Obj *a)
{
    Obj *o = Func_02007254(a->f64);

    Func_0200718e(o, a->f08, a->f0c + 0x240000, a->f10);
    o->f55 = 0;
    Func_02007176(o, (s32)Data_0200c288);
    Func_020073dc(83);
    a->f64 = 0;
    return 0;
}
