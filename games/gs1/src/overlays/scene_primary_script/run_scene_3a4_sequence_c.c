#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02004636();
void Func_0200468a();
void Func_02004692();
void Func_02004696();
s32 Func_020046a6();
void Func_020046b2();
void Func_020046ba();
void Func_020046c6();
void Func_020046e8();
void Func_02004710();
void Func_0200473e();
void Func_02004870();
void Func_020048da();

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

void FieldScene_RunScene3a4SequenceC(void)
{
    u32 i;
    s32 record;

    Func_020046b2();
    Call2(Func_02004692, 0x1528, 1);
    Func_02004870(125);
    if (Value1(Func_020046a6, 0x326) != 0) {
        Call6(Func_02004696, 15, 93, 1, 1, 16, 92);
        Call6(Func_0200468a, 47, 29, 16, 28, 1, 2);
        Call1(Func_020046e8, 0x326);
    } else {
        Call6(Func_020046c6, 17, 93, 1, 1, 16, 92);
        Call6(Func_020046ba, 46, 29, 16, 28, 1, 2);
        Call1(Func_02004710, 0x326);
    }
    Func_02004636(20);
    Func_020048da();
    Func_0200473e();
}
