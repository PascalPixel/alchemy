#include "types.h"

#define FieldScene_RunScene3a5_02000c6c Func_02000c6c

s32 Func_0200291c();
s32 Func_020029d4();
void Func_020029e6();
void Func_02002a02();
void Func_02002a26();
void Func_02002a8a();
void Func_02002a9a();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

void FieldScene_RunScene3a5_02000c6c(s32 a0)
{
    s32 i;
    s32 p8;
    s32 record;
    s32 value;
    s32 v5;
    s32 v6;

    p8 = a0;
    for (i = 0; i <= 2; i++) {
        value = Value0(Func_0200291c);
        v6 = (u32)((value << 1) + value) >> 16;
        v5 = v6 + 0x303;
        record = Value1(Func_020029d4, v5);
        if (record == 0) {
            Func_020029e6(v5);
            break;
        }
    }
    Func_02002a02();
    Func_02002a8a((s32)((s32)(((s32)p8 << 1) + p8) + v6) + 0x1a10);
    Func_02002a9a((v6 + 1), 0);
    Func_02002a26();
}
