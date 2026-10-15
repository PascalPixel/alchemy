#include "ov6a0.h"

void Func_02002b54(s32 x, s32 z, s32 layer, s32 value);
s32 Func_02002c3c(s32 x, s32 z, s32 layer, s32 value);

// Copies the map cells and sets the cell values that flags 0xa10 to 0xa15 call for.
void Func_020002f0(void)
{
    /* FAKEMATCH: every call goes through the call layer; called directly, the coordinates and flags are shared in saved registers (differ 117). */
    Call6((void (*)())Func_02002b2c, 0x5e, 0x3c, 9, 8, 0x5e, 0xa);
    Call4((void (*)())Func_02002b54, 0x2300000, 0xf00000, 0, -2);
    Call4((void (*)())Func_02002b54, 0x1f00000, 0x1000000, 0, -2);
    Call4((void (*)())Func_02002b54, 0x2100000, 0xa00000, 0, -2);
    Value4((s32 (*)())Func_02002c3c, 0x2600000, 0xa00000, 2, 0);
    Value4((s32 (*)())Func_02002c3c, 0x1f00000, 0xa00000, 2, 0);
    Value4((s32 (*)())Func_02002c3c, 0x1f00000, 0xe00000, 2, 0);
    if (Value1((s32 (*)())GameFlag_Test, 0xa10) || Value1((s32 (*)())GameFlag_Test, 0xa11))
        Call4((void (*)())Func_02002b54, 0x2300000, 0xf00000, 0, 0);
    if (Value1((s32 (*)())GameFlag_Test, 0xa12) || Value1((s32 (*)())GameFlag_Test, 0xa13))
        Call4((void (*)())Func_02002b54, 0x1f00000, 0x1000000, 0, 0);
    if (Value1((s32 (*)())GameFlag_Test, 0xa14) || Value1((s32 (*)())GameFlag_Test, 0xa15))
        Call4((void (*)())Func_02002b54, 0x2100000, 0xa00000, 0, 0);
    if (Value1((s32 (*)())GameFlag_Test, 0xa10)) {
        Call6((void (*)())Func_02002b2c, 0x60, 0x38, 7, 2, 0x60, 0xf);
        Call6((void (*)())Func_02002b2c, 0x6f, 0x33, 1, 1, 0x63, 0xf);
        if (Value1((s32 (*)())GameFlag_Test, 0xa13)) {
            Call6((void (*)())Func_02002b2c, 0x69, 0x33, 1, 3, 0x5f, 0xe);
            Value4((s32 (*)())Func_02002c3c, 0x1f00000, 0xe00000, 2, 0xec);
        }
        if (Value1((s32 (*)())GameFlag_Test, 0xa15))
            Call6((void (*)())Func_02002b2c, 0x6f, 0x35, 1, 1, 0x61, 0xa);
    } else if (Value1((s32 (*)())GameFlag_Test, 0xa11)) {
        Call6((void (*)())Func_02002b2c, 0x62, 0x33, 5, 1, 0x62, 0xa);
        Call6((void (*)())Func_02002b2c, 0x63, 0x34, 4, 5, 0x63, 0xb);
        Value4((s32 (*)())Func_02002c3c, 0x2600000, 0xa00000, 2, 0xec);
        if (Value1((s32 (*)())GameFlag_Test, 0xa12))
            Call6((void (*)())Func_02002b2c, 0x6a, 0x35, 1, 1, 0x5f, 0x10);
        if (Value1((s32 (*)())GameFlag_Test, 0xa14)) {
            Call6((void (*)())Func_02002b2c, 0x6d, 0x33, 3, 1, 0x5f, 0xa);
            Value4((s32 (*)())Func_02002c3c, 0x1f00000, 0xa00000, 2, 0xec);
        }
    } else {
        if (Value1((s32 (*)())GameFlag_Test, 0xa12))
            Call6((void (*)())Func_02002b2c, 0x6a, 0x35, 1, 1, 0x5f, 0x10);
        if (Value1((s32 (*)())GameFlag_Test, 0xa13))
            Call6((void (*)())Func_02002b2c, 0x6b, 0x35, 1, 1, 0x5f, 0x10);
        if (Value1((s32 (*)())GameFlag_Test, 0xa14))
            Call6((void (*)())Func_02002b2c, 0x6f, 0x34, 1, 1, 0x61, 0xa);
        if (Value1((s32 (*)())GameFlag_Test, 0xa15))
            Call6((void (*)())Func_02002b2c, 0x6f, 0x35, 1, 1, 0x61, 0xa);
    }
}
