#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020046d2();
void Func_02004726();
void Func_0200472e();
void Func_02004732();
s32 Func_02004742();
void Func_0200474e();
void Func_02004756();
void Func_02004762();
void Func_02004784();
void Func_020047ac();
void Func_020047da();
void Func_0200490c();
void Func_02004976();

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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
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

void FieldScene_RunScene3a4SequenceD(void)
{
    u32 i;
    s32 record;

    Func_0200474e();
    Call2(Func_0200472e, 0x1528, 1);
    Func_0200490c(125);
    if (Value1(Func_02004742, 0x327) != 0) {
        Call6(Func_02004732, 28, 82, 1, 1, 29, 81);
        Call6(Func_02004726, 47, 28, 29, 17, 1, 2);
        Call1(Func_02004784, 0x327);
    } else {
        Call6(Func_02004762, 30, 82, 1, 1, 29, 81);
        Call6(Func_02004756, 46, 28, 29, 17, 1, 2);
        Call1(Func_020047ac, 0x327);
    }
    Func_020046d2(20);
    Func_02004976();
    Func_020047da();
}
