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

extern Obj *Func_02005708(s32);
extern void Func_020056f6(void);
extern Obj *Func_02005716(s32);
extern void Func_020057f0(s32);
extern void Func_02005808(s32, s32);
extern void Func_02005780();
extern void Func_020058a4(void);
extern void Func_020058b0(void);
extern void Func_02005770(s32, s32);
extern void Func_020058a2(s32);
extern void Func_02005782(s32);
extern void Func_0200343a(s32);
extern void Func_020058de(void);
extern void Func_020058f2(void);
extern void Func_020057e4(void);

void Func_02001a28(void)
{
    u8 *state = Data_03001ebc;
    s32 best = 8;
    s32 bestd = 0x100000;
    s32 n = Data_02000240[125];
    Obj *p = Func_02005708(n);
    s32 i;
    s32 *q;
    s32 base;

    Func_020056f6();
    for (i = 8; i <= 66; i++) {
        Obj *o = Func_02005716(i);

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
    Func_020057f0(0x2085);
    Func_02005808(best, 0);
    q = (s32 *)(state + 448);
    *q = 512;
    *(s32 *)(state + 456) = 15;
    Func_02005780(20);
    Func_020058a4();
    Func_020058b0();
    base = n << 4;
    Func_02005770(base + 880, p->f08 >> 20);
    {
        s32 v = p->f10 >> 20;

        Func_02005780(base + 888, v);
    }
    n++;
    if (n > 3) {
        Func_020058a2(10);
        Func_02005782(282);
    } else {
        Func_0200343a(n);
        Func_020058de();
        Func_020058f2();
        *q = 0;
    }
    Func_020057e4();
}
