#include "types.h"

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 pad14[15];
    u8 f23;
};

extern struct S *Func_020048f4(void);
extern void Func_0200494a(s32 arg0, s32 arg1);
extern void Func_020048c8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

void Func_0200185c(s32 arg0, s32 arg1, s32 arg2) {
    struct S *p = Func_020048f4();

    if (p != 0) {
        s32 v;

        Func_0200494a(arg0, 3);
        v = 2;
        v |= p->f23;
        p->f23 = (u8)v;
        {
            s32 k5 = p->f10 >> 20;
            s32 k4 = (p->f08 >> 20) - 1;

            Func_020048c8(arg1, arg2, 3, 1, k4, k5);
        }
    }
}
