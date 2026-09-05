#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02006fbc();
void Func_02006fc4();
s32 Func_02006ff4();
void Func_02007012();
void Func_0200702c();
void Func_02007052();
s32 Func_02007058();
s32 Func_02007068();
s32 Func_02007070();
void Func_02007088();
void Func_02007222();

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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void Func_02001568(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    if (Value1(Func_02006ff4, 0x256) != 0) {
        Func_0200702c();
        Call1(Func_02007012, 0x256);
        record = Func_02007058(0);
        *(volatile s32 *)(record + 12) += 0x20000;
        rec7 = Value1(Func_02007068, 0);
        record = Value1(Func_02007070, 0);
        *(volatile s32 *)(rec7 + 60) = *(volatile s32 *)(record + 12);
        Func_02007052(5);
        Call6(Func_02006fbc, 7, 2, 5, 11, 1, 1);
        Func_02007222(217);
        Call3(Func_02006fc4, 0x200e03a, 9, 7);
        Func_02007088();
    }
}
