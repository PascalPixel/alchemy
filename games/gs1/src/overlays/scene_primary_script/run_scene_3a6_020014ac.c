#include "types.h"

#define FieldScene_RunScene3a6_020014ac Func_020014ac

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02001ac2();
void Func_02001ad2();
void Func_0200223c();
void Func_0200315c();
void Func_0200315e();
void Func_02003176();
void Func_02003186();
void Func_02003188();
void Func_020031c0();
s32 Func_020031c6();
void Func_020031c8();
s32 Func_020031d2();
void Func_02003200();
void Func_02003206();
s32 Func_0200320e();
void Func_02003214();
void Func_02003222();
void Func_02003224();
void Func_0200322c();
s32 Func_0200323e();
void Func_02003244();
void Func_02003246();
void Func_02003248();
void Func_0200324c();
void Func_0200326e();
void Func_02003270();
void Func_02003298();
void Func_0200329a();
void Func_020032a0();
void Func_02003312();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    return f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3a6_020014ac(void)
{
    u32 i;
    s32 record;
    s32 zero;

    Func_0200315e();
    Func_0200315c(10);
    Call3(Func_02003188, 0, 0x8000, 0x1999);
    Func_020031c8(0, 8);
    Func_02003176(15);
    Func_020031c0(0, 8, 0);
    Func_02003186(4);
    Call1(Func_02003246, 0x120);
    Func_0200324c(239);
    Call3(Func_020031c0, 9, 0x8000, 0x1999);
    Func_02003200(9, 2);
    zero = 0;
    *(u8 *)(Func_020031c6(9) + 85) = zero;
    record = Func_020031d2(9);
    *(volatile s32 *)(record + 68) = zero;
    Func_02003206(9, 12, 0);
    Func_02003214(0);
    Func_0200322c(0, 1);
    Func_02003222(9);
    Call1(Func_0200329a, 0x120);
    Func_020032a0(213);
    Func_02003248(9, 3);
    *(u8 *)(Func_0200320e(9) + 85) = 3;
    Func_02003246(9, 6, 0);
    Func_02003224(9);
    Func_0200223c();
    Func_02003270(9, 8);
    Func_02003298(9, 3);
    *(u8 *)(Func_0200323e(9) + 35) = 2;
    Value6(Func_02001ac2, 0, 12, 16, 1, 4, 0);
    Call6(Func_02001ad2, 0, 13, 16, 1, 4, 0);
    Call1(Func_02003244, 0x202);
    Func_02003312(240);
    Func_0200326e();
}
