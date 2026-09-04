#include "types.h"

#define FieldScene_RunScene3b4_02001bc4 Func_02001bc4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02003ace();
void Func_02004044();
void Func_020040cc();
void Func_020040e2();
s32 Func_020040e4();
void Func_02004124();
void Func_0200414e();

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

void FieldScene_RunScene3b4_02001bc4(void)
{
    u32 i;
    s32 record;

    Call1(Func_02004044, 0x2009e95);
    Func_0200414e(14, 0, 0);
    if (Value1(Func_020040e4, 0x207) != 0) {
        Call6(Func_020040cc, 58, 36, 1, 1, 45, 43);
    } else {
        Call6(Func_020040e2, 46, 43, 1, 1, 45, 43);
    }
    Func_02003ace();
    Call1(Func_02004124, 0x206);
}
