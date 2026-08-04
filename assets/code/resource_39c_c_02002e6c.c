#include "types.h"

extern s32 *Func_0200894e(s32);
extern void Func_020088b6(s32 *, s32);
extern void Func_02008a00(s32, s32);
extern void Func_02008afe(s32);
extern void Func_02006a98(s32, s32, s32);
extern void Func_02008b38(s32);
extern u32 Func_0200886c(void);
extern u32 Func_02008882(void);
extern u32 Func_02008894(void);
extern void Func_02003064(s32, s32, s32, s32, s32, s32, s32, void *);
extern void Func_020088b8(s32);
extern u8 Data_0200de70[];

struct Desc {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    s32 f14;
    s16 f18;
    s16 f1a;
    s32 f1c;
    s32 f20;
    s32 f24;
};

void Func_02002e6c(s32 a)
{
    struct Desc desc;
    s32 *p;
    u32 i;

    p = Func_0200894e(a);
    ((u8 *)p)[85] = 0;
    Func_020088b6(p, 0);
    {
        s32 h = a;

        Func_02008a00(h, 256);
    }
    Func_02008afe(0xdd);
    desc.f00 = 1;
    desc.f04 = 5;
    {
        s32 v = 286;

        desc.f18 = v;
    }
    desc.f1c = (s32)Data_0200de70;
    i = 0;
    do {
        if (i <= 31)
            Func_02006a98(p[2], p[3], p[4]);
        if ((i & 1) != 0) {
            Func_02008b38(0xf6);
            {
                s32 x = p[2] + (((Func_0200886c() * 24) >> 16) << 16) + 0xfff40000;
                s32 y = p[3] + (((Func_02008882() * 32) >> 16) << 16) + 0xfff00000;
                s32 w = (((Func_02008894() * 4) >> 16) << 15) + 0x8000;

                Func_02003064(x, y, p[4], 0, w, 0, 0x330000, &desc);
            }
        }
        p[3] += i * 0x1999;
        p[15] = p[3];
        Func_020088b8(2);
        i++;
    } while (i <= 47);
}
