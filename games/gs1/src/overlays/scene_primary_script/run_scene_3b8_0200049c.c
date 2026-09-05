#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
s32 Func_02004856();
void Func_02004868();
void Func_02004882();
void Func_020048a2();
void Func_020048c6();
void Func_020048d4();
void Func_020048fc();
void Func_02004940();
void Func_02004958();
void Func_0200495e();
void Func_02004968();
void Func_0200498e();
void Func_02004990();
void Func_020049ac();
void Func_020049c0();
void Func_020049d4();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void FieldScene_RunScene3b8_0200049c(s32 unused0, s32 a1)
{
    Func_02004882();
    Call1(Func_02004940, 0x2052);
    Func_02004958(a1, 0);
    if (Value1(Func_02004856, 0x968) == 0) {
        Call1(Func_02004868, 0x968);
        Func_020049d4();
        Func_020048a2(50);
        Call3(Func_0200498e, a1, 0x100, 70);
        Func_02004968(a1, 0, 40);
        Func_02004990(a1, 0);
        Func_020048c6(30);
        Func_0200495e(a1, 4);
        Func_020048d4(20);
        Func_020049ac(a1, 0);
        Call3(Func_020049c0, a1, 0x8000, 0);
    }
    Func_020048fc();
}
