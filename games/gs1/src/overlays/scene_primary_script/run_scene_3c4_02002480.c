#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02005532();
void Func_02005544();
void Func_02005574();
void Func_02005598();
s32 Func_020055ba();
void Func_020055bc();
s32 Func_020055c6();
s32 Func_020055ce();
s32 Func_020055ea();
s32 Func_020055f2();
s32 Func_0200560e();
s32 Func_02005616();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void FieldScene_RunScene3c4_02002480(void)
{
    s32 record;
    s32 p5;

    Call6(Func_02005532, 89, 49, 3, 2, 25, 49);
    Call6(Func_02005544, 89, 51, 8, 5, 25, 51);
    *(u8 *)(Func_020055ba(14) + 34) = 1;
    record = Value1(Func_020055c6, 12);
    p5 = *(volatile s32 *)(record + 8);
    record = Value1(Func_020055ce, 12);
    p5 = p5 >> 20;
    Call6(Func_02005574, 22, 52, 1, 1, p5, (*(volatile s32 *)(record + 16) >> 20));
    record = Value1(Func_020055ea, 13);
    p5 = *(volatile s32 *)(record + 8);
    record = Value1(Func_020055f2, 13);
    p5 = p5 >> 20;
    Call6(Func_02005598, 22, 52, 1, 1, p5, (*(volatile s32 *)(record + 16) >> 20));
    record = Value1(Func_0200560e, 14);
    p5 = *(volatile s32 *)(record + 8);
    record = Value1(Func_02005616, 14);
    p5 = p5 >> 20;
    Call6(Func_020055bc, 22, 52, 1, 1, p5, (*(volatile s32 *)(record + 16) >> 20));
}
