#include "types.h"

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    u8 f10[32];
    s32 f30;
    s32 f34;
    u8 f38[35];
    u8 f5b;
} Obj;

extern Obj *Func_02006c20(s32);
extern void Func_02006a3c(Obj *);
extern void Func_02006a24(Obj *, s32);
extern void Func_02006a60(Obj *, s32, s32, s32);
extern void Func_02006a6e(Obj *);
extern void Func_02006a3e(Obj *, s32);

void Func_02002e50(s32 a, s32 b, s32 c)
{
    Obj *o = Func_02006c20(a);

    if (o != 0) {
        s32 v = 0x14000;
        s32 z = 0;

        o->f30 = v;
        o->f34 = v >> 1;
        o->f5b = z;
        Func_02006a3c(o);
        Func_02006a24(o, 5);
        Func_02006a60(o, b << 16, o->f0c, c << 16);
        Func_02006a6e(o);
        Func_02006a3e(o, 1);
    }
}
