#include "types.h"

#define FieldScene_RunOpeningAuxiliarySequence Func_02000210

/* Audited retained auxiliary scene body.
 * The complete executable span preserves 19 calls, 0 loop(s), and 0 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

void Func_02001d14();
void Func_02001d1e();
s32 Func_02001d26();
void Func_02001d2a();
void Func_02001d34();
void Func_02001d42();
void Func_02001d4c();
void Func_02001d58();
void Func_02001d62();
void Func_02001d84();
void Func_02001d9a();
void Func_02001da2();
void Func_02001dac();
void Func_02001dc6();
void Func_02001dd8();
void Func_02001dde();
void Func_02001de8();
void Func_02001e12();
void Func_02001ed6();

/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */


static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    return f(a0, a1, a2, a3, a4, a5);
}

void Func_02000210(void)
{
    if (Value1(Func_02001d26, 0x202) != 0) {
        Value6(Func_02001d1e, 41, 86, 2, 6, 21, 57);
        Value1(Func_02001d14, 4);
        Value6(Func_02001d34, 43, 86, 2, 6, 21, 57);
        Value1(Func_02001d2a, 4);
        Value6(Func_02001d4c, 41, 86, 2, 6, 21, 58);
        Value1(Func_02001d42, 4);
        Value6(Func_02001d62, 43, 86, 2, 6, 21, 58);
        Value1(Func_02001d58, 4);
    }
    Call6(Func_02001d84, 2, 93, 1, 1, 24, 62);
    Call6(Func_02001d9a, 2, 94, 1, 1, 21, 55);
    Value6(Func_02001dac, 41, 86, 2, 6, 21, 59);
    Value1(Func_02001da2, 4);
    Call6(Func_02001dc6, 1, 93, 1, 1, 24, 62);
    Call6(Func_02001dd8, 3, 94, 1, 1, 21, 55);
    Value6(Func_02001de8, 43, 86, 2, 6, 21, 59);
    Value1(Func_02001dde, 4);
    Call2(Func_02001ed6, 10, 3);
    Call6(Func_02001e12, 19, 17, 1, 1, 22, 15);
}
