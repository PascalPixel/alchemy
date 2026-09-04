#include "types.h"

#define FieldScene_RunScene3a0_02000de8 Func_02000de8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_020021c0();
void Func_020021dc();
void Func_02002234();
void Func_0200223a();
void Func_02002240();
void Func_020022d6();

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

void FieldScene_RunScene3a0_02000de8(s32 a0)
{
    u32 i;
    s32 record;

    *(u8 *)(Func_020021c0(0) + 85) = 0;
    Call3(Func_020021dc, 0, 0x8000, 0x4000);
    if (a0 == 6) {
        Func_02002240(0, 2);
        Call3(Func_02002234, 0, 0, -16);
    } else {
        Call3(Func_0200223a, 0, 2, -16);
    }
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c8)) = 16;
    Func_020022d6(a0);
}
