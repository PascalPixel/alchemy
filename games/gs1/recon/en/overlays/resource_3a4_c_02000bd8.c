#include "types.h"

extern void Func_0200474e(void);
extern void Func_0200472e(s32, s32);
extern void Func_0200490c(s32);
extern s32 Func_02004742(s32);
extern void Func_02004732(s32, s32, s32, s32, s32, s32);
extern void Func_02004726(s32, s32, s32, s32, s32, s32);
extern void Func_02004784(s32);
extern void Func_02004762(s32, s32, s32, s32, s32, s32);
extern void Func_02004756(s32, s32, s32, s32, s32, s32);
extern void Func_020047ac(s32);
extern void Func_020046d2(s32);
extern void Func_02004976(void);
extern void Func_020047da(void);

void Func_02000bd8(void)
{
    s32 s0;
    s32 s1;

    Func_0200474e();
    Func_0200472e(0x1528, 1);
    Func_0200490c(125);
    if (Func_02004742(0x327) != 0) {
        s0 = 29;
        s1 = 81;
        Func_02004732(28, 82, 1, 1, s0, s1);
        s0 = 1;
        s1 = 2;
        Func_02004726(47, 28, 29, 17, s0, s1);
        Func_02004784(0x327);
    } else {
        s0 = 29;
        s1 = 81;
        Func_02004762(30, 82, 1, 1, s0, s1);
        s0 = 1;
        s1 = 2;
        Func_02004756(46, 28, 29, 17, s0, s1);
        Func_020047ac(0x327);
    }
    Func_020046d2(20);
    Func_02004976();
    Func_020047da();
}
