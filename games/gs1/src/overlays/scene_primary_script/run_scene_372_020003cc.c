#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_0200054e();
void Func_02004a68();
void Func_02004ae0();
void Func_02004b5a();
void Func_02004c86();

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

void FieldScene_RunScene372_020003cc(void)
{
    u32 i;
    s32 record;

    Func_02004c86(158);
    Call3(Func_02004a68, 0x200d78a, 35, 73);
    Call3(Func_02004b5a, 0, 102, 0x4b6);
    Func_02004ae0(3);
    Func_0200054e(12);
}
