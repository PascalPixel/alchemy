#include "types.h"

#define FieldScene_RunScene373_02000cd0 Func_02000cd0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02006b76();
void Func_02006c38();
void Func_02006c4c();
void Func_02006c90();
void Func_02006ce2();
void Func_02006d7e();
void Func_02006d82();
void Func_02006da4();
void Func_02006dae();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene373_02000cd0(void)
{
    u32 i;
    s32 record;

    Func_02006c90();
    Call3(Func_02006c38, 0x30000, 0x30000, 0x10000);
    Func_02006b76(10);
    Call3(Func_02006c4c, -1, -1, 0xe666);
    Call1(Func_02006d82, 0x1c9a);
    Func_02006da4(17, 0, 20);
    Func_02006d7e(17, 0, 20);
    Func_02006dae(17, 0);
    Func_02006ce2();
}
