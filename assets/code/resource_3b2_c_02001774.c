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

extern Obj *Func_0200481e(s32);
extern Obj *Func_02004826(s32);
extern void Func_02004814(void);
extern void Func_020047ee(Obj *, s32, s32, s32);
extern void Func_020047de(Obj *, s32);
extern void Func_0200481a(Obj *, s32, s32, s32);
extern void Func_02004816(Obj *, s32);
extern void Func_02004820(Obj *, s32);
extern void Func_02004946(s32);
extern void Func_0200484c(Obj *);
extern void Func_02004954(s32);
extern void Func_020048b8(void);

void Func_02001774(s32 a0, s32 a1, s32 a2)
{
    Obj *p;
    Obj *q;
    s32 x;
    s32 y;

    p = Func_0200481e(Data_02000240[125]);
    q = Func_02004826(a0);
    Func_02004814();
    {
        x = ((p->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Func_020047ee(p, x, p->f0c, y);
    }
    Func_020047de(p, 27);
    {
        x = ((q->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Func_0200481a(q, x, q->f0c, y);
    }
    if (a1 < 0 || a2 < 0) {
        Func_02004816(q, 4);
    } else {
        Func_02004820(q, 3);
    }
    Func_02004946(226);
    Func_0200484c(p);
    Func_02004954(288);
    Func_020048b8();
}
