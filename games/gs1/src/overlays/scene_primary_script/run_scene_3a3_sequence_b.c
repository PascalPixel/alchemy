#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001554();
s32 Func_0200157a();
void Func_02001594();
void Func_020015b2();
void Func_02001668();
void Func_02001674();
void Func_0200167e();
void Func_02001680();

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

void FieldScene_RunScene3a3SequenceB(void)
{
    u32 i;
    s32 record;
    u8 *p5;

    p5 = *(volatile s32 *)Data_03001ebc;
    Func_02001554();
    *(u8 *)(Func_0200157a(0) + 85) = 0;
    Func_0200167e(123);
    Call3(Func_020015b2, 0, 2, -16);
    Func_02001668(*(s16 *)(((s32)p5 + 0x16c)));
    Func_02001674();
    Func_02001680();
    Func_02001594();
}
