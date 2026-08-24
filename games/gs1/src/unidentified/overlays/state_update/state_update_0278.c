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

extern Obj *Func_02004016(s32);
extern Obj *Func_0200401e(s32);
extern void Func_0200400c(void);
extern void Func_02003f7e(Obj *, s32, s32, s32);
extern void Func_02003f56(Obj *, s32);
extern void Func_02003faa(Obj *, s32, s32, s32);
extern void Func_02003f8e(Obj *, s32);
extern void Func_02003f98(Obj *, s32);
extern void Func_020041ee(s32);
extern void Func_02003fdc(Obj *);
extern void Func_02003fac(Obj *, s32);
extern void Func_02004204(s32);
extern void Func_020040b8(void);

void Func_0200033c(s32 a0, s32 a1, s32 a2)
{
    Obj *p;
    Obj *q;
    s32 x;
    s32 y;

    p = Func_02004016(Data_02000240[125]);
    q = Func_0200401e(a0);
    Func_0200400c();
    {
        x = ((p->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Func_02003f7e(p, x, p->f0c, y);
    }
    Func_02003f56(p, 27);
    {
        x = ((q->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Func_02003faa(q, x, q->f0c, y);
    }
    if (a1 < 0 || a2 < 0) {
        Func_02003f8e(q, 4);
    } else {
        Func_02003f98(q, 3);
    }
    Func_020041ee(226);
    Func_02003fdc(p);
    Func_02003fac(q, 2);
    Func_02004204(288);
    Func_020040b8();
}
