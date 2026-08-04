#include "types.h"

typedef struct Obj {
    u32 f00[9];
    s32 f24;
    s32 f28;
    s32 f2c;
    s32 f30;
    s32 f34;
    u32 f38;
    s32 f3c;
    u32 f40;
} Obj;

extern Obj *Func_02006868(void);
extern void Func_0200677e(Obj *);

void Func_02002bac(void)
{
    Obj *o = Func_02006868();

    Func_0200677e(o);
    o->f24 = 0;
    o->f2c = 0;
    o->f38 = 0x80000000;
    o->f40 = 0x80000000;
}
