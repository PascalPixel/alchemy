#include "types.h"

#define FieldScene_RunPrimaryScript Func_02000180

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001638();
s32 Func_0200169e();
void Func_020016b6();
void Func_02001708();
void Func_02001714();
void Func_0200179e();
void Func_020017b4();

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

void FieldScene_RunPrimaryScript(void)
{
    u32 i;
    s32 record;

    Func_0200179e(188);
    Call3(Func_02001638, 0x2009788, 67, 6);
    *(u8 *)(Func_0200169e(0) + 85) = 0;
    Call3(Func_020016b6, 0, 0xcccc, 0x6666);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
    Func_02001714(0, 2);
    Call3(Func_02001708, 0, 0, -16);
    Func_020016b6(16);
    Func_020017b4(2);
}
