#include "types.h"

#define FieldScene_RunScene376_02000298 Func_02000298

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00001c40[];
extern u8 Data_03001ebc[];
void Func_02001438();
void Func_02001452();
void Func_020014e6();
void Func_020014fe();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene376_02000298(void)
{
    u32 i;
    s32 record;

    Func_02001438();
    Func_020014e6((s32)Data_00001c40);
    Call2(Func_020014fe, 0x800b, 0);
    Func_02001452();
}
