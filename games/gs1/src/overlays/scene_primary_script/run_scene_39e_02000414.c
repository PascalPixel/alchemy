#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_020047e2();
void Func_020047f4();
void Func_0200481a();
s32 Func_0200483e();
void Func_02004858();
void Func_020048ba();
s32 Func_020048e6();
void Func_0200491c();

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene39e_02000414(void)
{
    u32 i;
    s32 record;

    Func_020047f4();
    Call1(Func_020048ba, 0x178a);
    if (Value1(Func_020047e2, 0x890) != 0) {
        bump_step(4);
    }
    Value2(Func_020048e6, 8, 0);
    if (Value2(Func_0200483e, 0, 0) == 0) {
        Call1(Func_0200481a, 0x890);
    } else {
        bump_step(1);
    }
    Func_0200491c(8, 0);
    Func_02004858();
}
