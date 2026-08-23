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

extern u8 Data_0200ce74[];

extern Obj *Func_020087b4(s16);
extern void Func_020086be(Obj *, s32, s32, s32);
extern void Func_020086a6(Obj *, s32);
extern void Func_02008964(s32);

s32 Func_02003d88(Obj *a)
{
    Obj *o = Func_020087b4(a->f64);

    Func_020086be(o, a->f08, a->f0c + 0x240000, a->f10);
    o->f55 = 0;
    Func_020086a6(o, (s32)Data_0200ce74);
    Func_02008964(83);
    a->f64 = 0;
    return 0;
}
