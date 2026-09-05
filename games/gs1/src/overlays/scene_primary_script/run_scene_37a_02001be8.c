#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02004252();
void Func_02004270();
void Func_02004606();
void Func_02004612();
void Func_02004626();
void Func_0200462c();
void Func_02004630();
void Func_02004636();
void Func_02004654();
void Func_0200465e();
void Func_02004662();
void Func_02004682();
void Func_0200468c();
void Func_02004698();
void Func_0200469a();
void Func_020046a8();
void Func_020046aa();
void Func_020046e2();

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

void FieldScene_RunScene37a_02001be8(void)
{
    u32 i;
    s32 record;

    Func_020046e2(21);
    Call3(Func_02004606, 0, 0x178, 184);
    Func_02004626(0, 0);
    Call3(Func_0200462c, 16, 0x1780000, 0xb80000);
    Call3(Func_02004612, 16, 0x10000, 0x8000);
    Call3(Func_02004636, 16, 0x188, 168);
    Call3(Func_020046aa, 16, 0x8000, 30);
    Func_02004662(16, 1);
    Call1(Func_02004698, 0x102b);
    Func_02004682(16, 4, 30);
    Func_02004252(16, 6);
    Func_0200469a(0, 2);
    Func_02004630(6);
    Func_02004698(16, 3);
    Func_02004270(16, 6);
    Call3(Func_0200468c, 16, 0x178, 184);
    Call3(Func_020046a8, 16, 0x6480000, 0x6480000);
    Func_0200465e(4);
    Call1(Func_02004654, 0x811);
}
