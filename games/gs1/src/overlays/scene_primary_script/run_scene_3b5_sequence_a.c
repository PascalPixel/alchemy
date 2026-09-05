#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020015f4();
void Func_02001618();
void Func_02001642();
s32 Func_0200164e();
void Func_0200167e();
void Func_02001688();
void Func_020016a4();
void Func_020016ae();
void Func_020016bc();
void Func_020016be();
s32 Func_020016ce();
void Func_020016da();
void Func_020016fa();
void Func_02001740();
void Func_02001748();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
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

void FieldScene_RunScene3b5SequenceA(void)
{
    u32 i;
    s32 record;

    Func_020015f4();
    Call3(Func_02001642, 0, 0x130, 0x138);
    Call3(Func_020016ce, 0, 0xc000, 0);
    Call3(Func_020016da, 28, 0x4000, 0);
    Func_02001618(20);
    Call1(Func_020016be, 0xe3d);
    Value2(Func_020016ce, 28, 0);
    if (Value2(Func_0200164e, 0, 0) == 0) {
        bump_step(1);
        Func_020016fa(28, 0);
        Call3(Func_02001688, 28, 0x10000, 0x8000);
        Call3(Func_020016ae, 28, 0x140, 0x130);
        Call3(Func_020016bc, 28, 0x13c, 0x148);
        Call3(Func_02001748, 28, 0xa000, 0);
        Call1(Func_0200167e, 0x8c1);
    } else {
        Func_02001740(28, 0);
    }
    Func_020016a4();
}
