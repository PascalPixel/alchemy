#include "types.h"

#define OverlayObject_ResetMotionFields Func_02002bac
#define OverlayObject_PlaceWithScale14000 Func_02002e50
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

typedef struct Obj_02002e50 {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    u8 f10[32];
    s32 f30;
    s32 f34;
    u8 f38[35];
    u8 f5b;
} Obj_02002e50;

Obj *Func_02006868(void);
void Func_0200677e(Obj *);
Obj *Func_02006c20(s32);
void Func_02006a3c(Obj *);
void Func_02006a24(Obj *, s32);
void Func_02006a60(Obj *, s32, s32, s32);
void Func_02006a6e(Obj *);
void Func_02006a3e(Obj *, s32);

void OverlayObject_ResetMotionFields(void)
{
    Obj *o = Func_02006868();

    Func_0200677e(o);
    o->f24 = 0;
    o->f2c = 0;
    o->f38 = 0x80000000;
    o->f40 = 0x80000000;
}

void OverlayObject_PlaceWithScale14000(s32 a, s32 b, s32 c)
{
    Obj_02002e50 *o = Func_02006c20(a);

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
