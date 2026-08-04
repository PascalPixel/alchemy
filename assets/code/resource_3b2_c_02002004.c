#include "types.h"

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct S *Func_02005094(s32 arg0);
extern struct S *Func_0200509e(s32 arg0);
extern struct S *Func_020050d6(s32 arg0);
extern void Func_0200379c(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020037b0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005000(s32 arg0);
extern void Func_0200508c(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_0200509c(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

void Func_02002004(void) {
    s32 x = Func_02005094(12)->f08 >> 20;
    s32 y = Func_0200509e(12)->f10 >> 20;

    if (x == 24) {
        Func_0200379c(12, 96, 0);
        Func_020037b0(12, 96, 0);
    } else if (x == 34) {
        Func_020037b0(12, 32, 0);
    } else if (x == 36) {
        return;
    }
    Func_02005000(2);
    {
        s32 k = Func_020050d6(12)->f08 >> 20;
        s32 m = y - 1;

        Func_0200508c(x, m, 1, 3, k, m);
    }
    Func_0200509c(0, 0, 1, 3, x, y - 1);
}
