#include "TYPES.H"


void Func_02003bbc();
void Func_02003dae();
void Func_02003dc6();
void Func_02003dea();
void Func_02003e28();
void Func_02003e56();
void Func_02003e5c();
s32 Func_02003e6c();
s32 Func_02003e74();
void Func_02003ea4();
void Func_02003ed6();
void Func_02003ede();
void Func_02003f14();
void Func_02003f26();
void Func_02003fe2();
void Func_0200401a();
void Func_02004084();
void Func_0200408e();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* NONMATCHING: 188 of 188 bytes, 2 halfword edits (2026-09-24). sched2 orders the two
 * shifts of ((d >> 14) << 14) + 0x40000 the other way round: the reference
 * shifts d before it finishes building 0x40000. */
void Func_020002cc(s32 a0)
{
    u32 i;
    s32 rec7;
    s32 rec8;
    s32 record;
    s32 v3;

    rec8 = Value1(Func_02003e6c, 0);
    rec7 = Value1(Func_02003e74, 8);
    Func_02003fe2();
    Func_02003e56();
    Func_02003ede(0, 22);
    Func_02003e5c(10);
    Func_0200401a(152);
    Func_02003ea4(0, 0x33333, 0x19999);
    v3 = (*(s32 *)(rec7 + 12) - *(s32 *)(rec8 + 12));
    if ((*(s32 *)(rec7 + 12) - *(s32 *)(rec8 + 12)) < 0) {
        v3 = (*(s32 *)(rec8 + 12) - *(s32 *)(rec7 + 12));
    }
    *(s32 *)(rec8 + 40) = 0x40000 + ((v3 >> 14) << 14);
    Func_02003f14(0, 7);
    Func_02003e28(rec8, *(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), *(s32 *)(rec7 + 16));
    Func_02003dae(10);
    *(u8 *)(*(s32 *)(rec8 + 80) + 9) |= 12;
    Func_02003f26(0);
    for (;;) {
        if (!((*(s32 *)(rec7 + 12) >> 14) < (*(s32 *)(rec8 + 12) >> 14))) break;
        Func_02003dc6(1);
    }
    Func_02003ed6();
    Func_02004084(159);
    Func_02003bbc(a0, 0);
    Func_02003dea(20);
    Func_0200408e();
}
