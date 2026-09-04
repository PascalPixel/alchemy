#include "types.h"

#define FieldScene_RunScene373_02000e84 Func_02000e84

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02006dc0();
void Func_02006ebc();
void Func_02006fa2();
void Func_02006fd6();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene373_02000e84(void)
{
    u32 i;
    s32 record;

    Func_02006fd6(158);
    Call3(Func_02006dc0, 0x200f570, 45, 39);
    Call3(Func_02006ebc, 0, 0x106, 0x325);
    Func_02006fa2(6);
}
