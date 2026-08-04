#include "types.h"

struct Obj {
    u8 filler00[8];
    s32 f08;
    s32 f0c;
    u8 filler10[8];
    s32 f18;
    s32 f1c;
    s32 f20;
    s32 f24;
    s32 f28;
    u8 filler2c[56];
    s16 f64;
};

extern void Func_0200727e(void);

void Func_020036f8(struct Obj *p) {
    s16 *h;
    s32 v;
    s32 a;
    s32 b;

    h = &p->f64;
    v = *h;
    if (v == 0) {
        Func_0200727e();
    } else if (v == 1) {
        p->f24 = 0;
        p->f28 = 0;
        p->f08 = 0;
        p->f0c = 0;
    } else {
        p->f18 += 0x800;
        p->f1c += 0x800;
    }
    p->f08 += p->f24;
    p->f0c += p->f28;
    a = p->f24;
    b = p->f28;
    p->f24 = a - a / 256;
    p->f28 = b - b / 16;
    { s32 t = *(u16 *)h; t -= 1; *(u16 *)h = t; }
}
