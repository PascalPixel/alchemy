#include "types.h"

struct S {
    s32 f00;
    u16 f04;
    u16 f06;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[20];
    s32 f28;
    u8 pad2c[4];
    s32 f30;
    s32 f34;
    u8 pad38[29];
    u8 f55;
};

struct V {
    s32 a;
    s32 b;
    s32 c;
};

extern void Func_02004a60(s32 arg0, s32 arg1, struct V *arg2);
extern s32 Func_02004ac0(struct S *arg0, struct V *arg1);
extern void Func_02004b08(void);
extern void Func_02004a98(struct S *arg0, s32 arg1);
extern void Func_02004a5e(s32 arg0);
extern void Func_02004bc4(s32 arg0);
extern void Func_02004aac(struct S *arg0, s32 arg1);
extern void Func_02004b0e(struct S *arg0, s32 arg1);
extern void Func_02004b7c(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02004ae4(struct S *arg0, s32 arg1);
extern void Func_02004b2c(struct S *arg0, s32 arg1);
extern void Func_02004b74(void);

s32 Func_02001a44(struct S *p) {
    u8 *q = &p->f55;
    s32 saved = *q;
    struct V v;

    v.a = (p->f08 & 0xfff00000) + 0x80000;
    v.b = p->f0c;
    v.c = (p->f10 & 0xfff00000) + 0x80000;
    {
        s32 m = (p->f06 + 0x2000) & 0xc000;

        Func_02004a60(0x200000, m, &v);
    }
    if (Func_02004ac0(p, &v) == 0) {
        s32 t;

        Func_02004b08();
        Func_02004a98(p, 6);
        Func_02004a5e(6);
        Func_02004bc4(152);
        Func_02004aac(p, 7);
        p->f30 = 0x30000;
        p->f34 = 0x20000;
        p->f28 = 0x40000;
        t = 126;
        t &= *q;
        *q = (u8)t;
        Func_02004b0e(p, 0);
        {
            s16 *h = (s16 *)&v;

            Func_02004b7c(0, h[1], h[5]);
        }
        Func_02004ae4(p, 6);
        Func_02004b2c(p, 1);
        *q = (u8)saved;
        Func_02004b74();
        return 1;
    }
    return 0;
}
