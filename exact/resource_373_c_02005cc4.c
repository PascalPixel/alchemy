#include "types.h"

struct S {
    s32 pad0[2];
    s32 f8;
    s32 fc;
    s32 f10;
    s32 pad14;
    s32 f18;
    s32 f1c;
    s32 pad20[17];
    s16 f64;
    s16 pad66;
    s32 *f68;
};

extern void Func_0200bbec(s32);
extern s32 Func_0200bb9c(s32);

void Func_02005cc4(struct S *p)
{
    s32 *q = p->f68;
    s32 v = ++p->f64;
    if (v > 31) {
        Func_0200bbec((s32)p);
    } else {
        s32 r = Func_0200bb9c(v << 10);
        s32 t;
        p->f18 = r;
        p->f1c = r;
        p->f8 = q[2];
        p->fc += 0x10000;
        t = 0x10000 - r;
        p->f10 = q[4] + t * 5 + 0x80000;
    }
}
