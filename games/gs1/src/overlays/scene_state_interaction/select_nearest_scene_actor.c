#include "types.h"

extern u8 *Data_03001ebc;
extern s32 Data_02000240[];

typedef struct Sub {
    u8 pad00[0x28];
    s16 *f28;
} Sub;

typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[0x3c];
    Sub *f50;
    u8 f54;
} Obj;

extern Obj *Func_02005c38(s32);
extern void Func_02005c26(void);
extern Obj *Func_02005c46(s32);
extern void Func_02005d28(s32);
extern void Func_02005d40(s32, s32);
extern void Func_02005cb0();
extern void Func_02005dec(void);
extern void Func_02005df8(void);
extern void Func_02005ca0(s32, s32);
extern void Func_02005de2(s32);
extern void Func_02005caa(s32);
extern void Func_0200396a(s32);
extern void Func_02005e26(void);
extern void Func_02005e3a(void);
extern void Func_02005d14(void);

void Func_02001cc0(void)
{
    u8 *state = Data_03001ebc;
    s32 best = 8;
    s32 bestd = 0x100000;
    s32 n = Data_02000240[125];
    Obj *p = Func_02005c38(n);
    s32 i;
    s32 *q;
    s32 base;

    Func_02005c26();
    for (i = 8; i <= 66; i++) {
        Obj *o = Func_02005c46(i);

        if (o != 0 && o->f54 == 1 && *o->f50->f28 == 165) {
            s32 dx = (p->f08 - o->f08) / 65536;
            s32 dy = (p->f10 - o->f10) / 65536;

            if (dy <= 0) {
                s32 a = dx;
                s32 d;

                if (a < 0) a = -a;
                if (dy < 0) dy = -dy;
                d = a + dy;
                if (d < bestd) {
                    best = i;
                    bestd = d;
                }
            }
        }
    }
    Func_02005d28(0x2085);
    Func_02005d40(best, 0);
    q = (s32 *)(state + 448);
    *q = 512;
    *(s32 *)(state + 456) = 15;
    Func_02005cb0(20);
    Func_02005dec();
    Func_02005df8();
    base = n << 4;
    Func_02005ca0(base + 880, p->f08 >> 20);
    {
        s32 v = p->f10 >> 20;

        Func_02005cb0(base + 888, v);
    }
    n++;
    if (n > 3) {
        Func_02005de2(10);
        Func_02005caa(282);
    } else {
        Func_0200396a(n);
        Func_02005e26();
        Func_02005e3a();
        *q = 0;
    }
    Func_02005d14();
}
