#include "types.h"

#define FieldScene_RunScene372_02000ec4 Func_02000ec4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_0200558c();
void Func_0200559e();
void Func_020055b0();
void Func_020055c2();
void Func_020055d4();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

void FieldScene_RunScene372_02000ec4(void)
{
    u32 i;
    s32 record;

    Call6(Func_0200558c, 29, 64, 1, 1, 21, 57);
    Call6(Func_0200559e, 29, 64, 1, 1, 21, 58);
    Call6(Func_020055b0, 29, 64, 1, 1, 22, 58);
    Call6(Func_020055c2, 29, 64, 1, 1, 20, 58);
    Call6(Func_020055d4, 28, 20, 1, 1, 20, 57);
}
