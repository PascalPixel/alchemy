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

extern struct S *Func_02004948(void);
extern void Func_0200499e(s32 arg0, s32 arg1);
extern void Func_0200491c(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

void Func_020018b0(s32 arg0, s32 arg1, s32 arg2) {
    struct S *p = Func_02004948();

    if (p != 0) {
        s32 v;

        Func_0200499e(arg0, 3);
        v = 2;
        v |= p->f23;
        p->f23 = (u8)v;
        {
            s32 k4 = p->f08 >> 20;
            s32 k5 = (p->f10 >> 20) - 1;

            Func_0200491c(arg1, arg2, 1, 3, k4, k5);
        }
    }
}
