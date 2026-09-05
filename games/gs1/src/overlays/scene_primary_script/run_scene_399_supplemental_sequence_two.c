#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02003948();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunSupplementalSequenceTwo(s32 a0)
{
    s32 remaining;

    *(volatile s32 *)(a0 + 8) += (*(s16 *)(a0 + 100) << 8);
    *(volatile s32 *)(a0 + 12) += 0x8000;
    *(volatile s32 *)(a0 + 24) += 0x7ae;
    *(volatile s32 *)(a0 + 28) += 0x7ae;
    *(volatile u16 *)(a0 + 100) += 2;
    remaining = *(volatile s32 *)(a0 + 104) - 1;
    *(volatile s32 *)(a0 + 104) = remaining;
    if (remaining == 0) {
        Func_02003948();
    }
}
