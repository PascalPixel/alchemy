#include "types.h"
#define NULL ((void *)0)

struct Sub {
    u8 pad00[8];
    s32 f08;
    u8 pad0c[4];
    s32 f10;
};

struct Obj {
    u8 pad00[8];
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[4];
    s32 f18;
    s32 f1c;
    u8 pad20[0x44];
    u16 f64;
    u8 pad66[2];
    struct Sub *f68;
};

s32 Func_02004a1c(s32);
void Func_02004a34(struct Obj *);

void Func_020023ec(struct Obj *p) {
    struct Sub *q;
    s32 v;
    s32 r;
    s32 k;

    q = p->f68;
    p->f64 = p->f64 + 1;
    v = (s16) p->f64;
    if (v > 31) {
        Func_02004a34(p);
        return;
    }
    r = Func_02004a1c(v << 10);
    p->f18 = r;
    p->f1c = -r;
    p->f08 = q->f08;
    k = 0x10000;
    p->f0c = p->f0c + k;
    k = k - r;
    p->f10 = q->f10 - (k * 4 + k) + 0x100000;
}
