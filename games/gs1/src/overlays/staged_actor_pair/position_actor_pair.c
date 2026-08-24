#include "types.h"

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[28];
    s32 f30;
    s32 f34;
} Obj;

extern s32 Data_02000240[];

extern Obj *Func_020034b6();
extern Obj *Func_020034be(s32);
extern void Func_020034b4(void);
extern void Func_02003496(Obj *, s32, s32, s32);
extern void Func_0200347e(Obj *, s32);
extern void Func_020034c2(Obj *, s32, s32, s32);
extern void Func_020034c0(Obj *, s32);
extern void Func_020034ee(Obj *);
extern void Func_0200354a(void);

void Func_020017d8(s32 a0, s32 a1, s32 a2)
{
    Obj *p;
    Obj *q;
    s32 x;
    s32 y;

    p = Func_020034b6(Data_02000240[125]);
    q = Func_020034be(a0);
    Func_020034b4();
    {
        x = ((p->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Func_02003496(p, x, p->f0c, y);
    }
    Func_0200347e(p, 27);
    {
        x = ((q->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Func_020034c2(q, x, q->f0c, y);
    }
    if (a1 < 0 || a2 < 0) {
        Func_020034b6(q, 4);
    } else {
        Func_020034c0(q, 3);
    }
    Func_020034ee(p);
    Func_0200354a();
}
