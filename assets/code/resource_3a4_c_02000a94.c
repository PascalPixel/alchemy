#include "types.h"

extern s32 Func_020045ec(s32);
extern void Func_020045dc(s32, s32, s32, s32, s32, s32);
extern void Func_020045d0(s32, s32, s32, s32, s32, s32);
extern void Func_0200462e(s32);
extern void Func_0200460c(s32, s32, s32, s32, s32, s32);
extern void Func_02004600(s32, s32, s32, s32, s32, s32);
extern void Func_02004656(s32);

void Func_02000a94(void)
{
    s32 s0;
    s32 s1;

    if (Func_020045ec(0x325) != 0) {
        s0 = 11;
        s1 = 73;
        Func_020045dc(12, 72, 1, 1, s0, s1);
        s0 = 1;
        s1 = 2;
        Func_020045d0(48, 32, 11, 4, s0, s1);
        Func_0200462e(0x325);
    } else {
        s0 = 11;
        s1 = 73;
        Func_0200460c(10, 72, 1, 1, s0, s1);
        s0 = 1;
        s1 = 2;
        Func_02004600(49, 32, 11, 4, s0, s1);
        Func_02004656(0x325);
    }
}
