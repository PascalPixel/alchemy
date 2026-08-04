#include "types.h"

struct T {
    u8 pad00[30];
    u16 f1e;
};

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    u8 pad10[32];
    s32 f30;
    s32 f34;
    s32 f38;
    s32 f3c;
    u8 pad40[16];
    struct T *f50;
};

extern s32 Func_02005e42(s32 arg0);
extern s32 Func_02005e58(s32 arg0);
extern s32 Func_02005e72(s32 arg0);
extern s32 Func_02005e70(void);
extern s32 Func_02005e76(void);

s32 Func_02002e70(struct S *p) {
    struct T *t = p->f50;
    s32 a = Func_02005e42(p->f30) * 2;
    s32 b;
    s32 c;

    if (a > 0) {
        a = -a;
    }
    p->f08 = p->f38 + Func_02005e58(p->f30) * 2;
    p->f0c = p->f3c + a;
    t->f1e = (u16)(Func_02005e72(p->f30 + 0x8000) / 8);
    c = Func_02005e70();
    b = Func_02005e76();
    p->f30 += (((u32)(c << 9)) >> 16) + (((u32)(b << 9)) >> 16) + 0x400;
    return 0;
}
