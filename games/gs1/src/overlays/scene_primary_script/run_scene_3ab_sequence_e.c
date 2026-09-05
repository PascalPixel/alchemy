#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02002644();
void Func_0200264c();
void Func_02002660();
void Func_02002662();
s32 Func_02002690();
void Func_02002692();
void Func_020026a2();
void Func_020026b8();
void Func_020026da();
void Func_020026e6();
void Func_0200279e();
void Func_020027ba();
void Func_020027c6();
void Func_020027ca();
void Func_020027e0();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3abSequenceE(void)
{
    u32 i;
    s32 record;
    s32 v5;
    s32 v6;

    Call1(Func_02002644, 0x242);
    Func_02002660();
    Call3(Func_02002692, 0, 0x3333, 0x1999);
    *(u8 *)(Func_02002690(0) + 85) = 0;
    Func_020026e6(0, 2);
    Call3(Func_020027ca, 0, 0, -8);
    v5 = 41;
    v6 = 4;
    Func_020027e0(158);
    Call6(Func_0200264c, 53, 4, 2, 2, v5, v6);
    Func_020026a2(10);
    Call6(Func_02002662, 53, 6, 2, 2, v5, v6);
    Func_020026b8(10);
    Func_0200279e(1);
    Func_020027ba();
    Func_020027c6();
    Func_020026da();
}
