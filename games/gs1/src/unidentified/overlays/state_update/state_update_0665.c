#include "types.h"

extern s32 Data_03001e40;
extern s32 Data_0200b398;
extern u8 Data_0200b2d0[];

struct Sub { u8 pad00[9]; u8 f09; u8 pad0a[28]; u8 f26; };
struct Obj {
    u8 pad00[0x18];
    s32 f18;
    u8 pad1c[7];
    u8 f23;
    u8 pad24[12];
    s32 f30;
    s32 f34;
    u8 pad38[24];
    struct Sub *f50;
    u8 pad54[1];
    u8 f55;
};

void Func_02005506(s32);
struct Obj *Func_020053a6(s32, s32, s32, s32);
void Func_020053d6(struct Obj *, s32);
void Func_0200540e(struct Obj *, s32, s32, s32);
void Func_020053f6(struct Obj *, u8 *);

void Func_020027c8(void) {
    struct Obj *p;
    struct Sub *q;
    s32 f;
    s32 v;
    s32 w;
    s32 c1 = 0x00e70000;
    s32 c2 = 0x01cc0000;
    s32 c3 = 0x00e70000;
    s32 c4 = 0x02700000;

    f = Data_03001e40 & 7;
    if (f != 0) return;
    if (Data_0200b398 != 0) Func_02005506(200);
    p = Func_020053a6(26, c1, 0, c2);
    if (p == 0) return;
    q = p->f50;
    q->f26 = f;
    v = 0xfe;
    v &= p->f23;
    p->f23 = v;
    w = ~12;
    w &= q->f09;
    w |= 4;
    q->f09 = w;
    p->f18 = 0x1999;
    p->f30 = 0x80000;
    p->f34 = 0x80000;
    p->f55 = f;
    Func_020053d6(p, 2);
    Func_0200540e(p, c3, 0, c4);
    Func_020053f6(p, Data_0200b2d0);
}
