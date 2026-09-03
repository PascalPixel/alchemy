#include "types.h"

#define FieldScene_RunScene373SequenceA Func_02000f8c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02006ec8();
void Func_02006fc4();
void Func_020070aa();
void Func_020070de();

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
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene373SequenceA(void)
{
    u32 i;
    u8 *record;

    Func_020070de(158);
    Call3(Func_02006ec8, 0x200f586, 52, 76);
    Call3(Func_02006fc4, 0, 0x176, 0x4d6);
    Func_020070aa(9);
}
