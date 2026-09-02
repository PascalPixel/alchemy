#include "types.h"

#define FieldScene_RunSupplementalSequenceTwo Func_020049a0

/* Audited retained supplemental scene body.
 * The complete production span preserves 49 calls, 0 loop(s), and 0 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

s32 Func_02009fb6();
s32 Func_02009fc2();
s32 Func_02009fd0();
void Func_0200a00c();
void Func_0200a036();
void Func_0200a046();
void Func_0200a070();
void Func_0200a08a();
void Func_0200a090();
void Func_0200a098();
void Func_0200a09e();
void Func_0200a0a6();
void Func_0200a0b0();
void Func_0200a0b4();
void Func_0200a0b6();
void Func_0200a0cc();
void Func_0200a0ce();
void Func_0200a0d8();
void Func_0200a0e2();
void Func_0200a0fe();
void Func_0200a10c();
void Func_0200a112();
void Func_0200a118();
void Func_0200a122();
void Func_0200a124();
void Func_0200a12a();
void Func_0200a12c();
void Func_0200a132();
void Func_0200a13a();
void Func_0200a142();
void Func_0200a154();
void Func_0200a15a();
void Func_0200a15c();
void Func_0200a162();
void Func_0200a16e();
void Func_0200a17c();
void Func_0200a18c();
void Func_0200a1a4();
void Func_0200a1a6();
void Func_0200a1b4();
void Func_0200a1c2();
void Func_0200a1d6();
void Func_0200a1da();
void Func_0200a20a();
void Func_0200a21a();
void Func_0200a222();
void Func_0200a232();

/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */


static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

void Func_020049a0(void)
{
    s32 base;

    if (Value1(Func_02009fb6, 2369) != 0) {
        if (Value1(Func_02009fc2, 2382) == 0 && Value1(Func_02009fd0, 788) == 0) {
        base = 9569;
        Call1(Func_0200a0b6, base);
        Call2(Func_0200a0ce, 24, 0);
        Value2(Func_0200a0a6, 24, 1);
        Value1(Func_0200a00c, 30);
        Call3(Func_0200a046, 24, 6553, 3276);
        Value3(Func_0200a08a, 24, -4, 0);
        Value1(Func_0200a098, 24);
        Value2(Func_0200a0b0, 24, 3);
        Value1(Func_0200a036, 60);
        Call3(Func_0200a070, 24, 13107, 6553);
        Call3(Func_0200a0b4, 24, -6, 0);
        Value3(Func_0200a0fe, 24, 0, 0);
        Value1(Func_0200a0cc, 24);
        Value1(Func_0200a122, base + 1);
        Call2(Func_0200a13a, 24, 0);
        Call2(Func_0200a112, 24, 1);
        Value3(Func_0200a124, 25, 24, 0);
        Value1(Func_0200a142, base + 2);
        Value2(Func_0200a15a, 24, 0);
        Value1(Func_0200a090, 70);
        Value2(Func_0200a118, 25, 3);
        Value1(Func_0200a09e, 60);
        Call3(Func_0200a0d8, 25, 26214, 13107);
        Value3(Func_0200a10c, 25, 880, 112);
        Value1(Func_0200a12a, 25);
        Value3(Func_0200a1a6, 25, 53248, 0);
        Value1(Func_0200a18c, base + 3);
        Call2(Func_0200a1a4, 24, 0);
        Value2(Func_0200a15c, 24, 3);
        Value1(Func_0200a0e2, 70);
        Value3(Func_0200a154, 24, 8, 0);
        Value1(Func_0200a162, 24);
        Value2(Func_0200a17c, 24, 5);
        Value1(Func_0200a1c2, base + 4);
        Call2(Func_0200a1da, 24, 0);
        Value3(Func_0200a16e, 0, 896, 120);
        Value1(Func_0200a18c, 0);
        Value3(Func_0200a1d6, 0, 25, 0);
        Value1(Func_0200a12c, 60);
        Call2(Func_0200a1b4, 25, 3);
        Value1(Func_0200a13a, 30);
        Value1(Func_0200a132, 788);
        } else {
            Call1(Func_0200a20a, 9575);
            Call2(Func_0200a222, 24, 0);
        }
    } else {
        Call1(Func_0200a21a, 0x244d);
        Call2(Func_0200a232, 24, 0);
    }
}
