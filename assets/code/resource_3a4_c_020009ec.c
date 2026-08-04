#include "types.h"



extern s32 Func_02004544(s32);
extern void Func_02004534(s32, s32, s32, s32, s32, s32);
extern void Func_02004528(s32, s32, s32, s32, s32, s32);
extern void Func_02004586(s32);
extern void Func_02004564(s32, s32, s32, s32, s32, s32);
extern void Func_02004558(s32, s32, s32, s32, s32, s32);
extern void Func_020045ae(s32);

void Func_020009ec(void)
{
    s32 s0;
    s32 s1;

    if (Func_02004544(0x323) != 0) {
        s0 = 24;
        s1 = 80;
        Func_02004534(2, 0, 1, 1, s0, s1);
        s0 = 1;
        s1 = 2;
        Func_02004528(2, 1, 24, 11, s0, s1);
        Func_02004586(0x323);
    } else {
        s0 = 24;
        s1 = 80;
        Func_02004564(0, 0, 1, 1, s0, s1);
        s0 = 1;
        s1 = 2;
        Func_02004558(0, 1, 24, 11, s0, s1);
        Func_020045ae(0x323);
    }
}
