#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02007bfa();
void Func_02007c0e();
void Func_02007c3c();
void Func_02007c3e();
void Func_02007c4c();
void Func_02007cb6();
void Func_02007d70();
void Func_02007d7c();
void Func_02007d88();
void Func_02007d94();
s32 Func_02007da0();
void Func_02007db8();
void Func_02007dc6();
void Func_02007dd2();

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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3bf_020025f8(void)
{
    u32 i;
    s32 record;

    Call3(Func_02007bfa, 0x40000, 0x40000, 0x10000);
    Func_02007db8(141);
    Func_02007c3e(80);
    Call1(Func_02007dc6, 0x120);
    Func_02007c4c(5);
    Func_02007dd2(145);
    Call6(Func_02007c0e, 16, 75, 7, 4, 26, 55);
    Call3(Func_02007c3c, -1, -1, 0xe666);
    Call3(Func_02007d70, 0, 0x100, 0);
    Call3(Func_02007d7c, 1, 0x100, 0);
    Call3(Func_02007d88, 2, 0x100, 0);
    Call3(Func_02007d94, 3, 0x100, 0);
    Value3(Func_02007da0, 12, 0x100, 0);
    Func_02007cb6(60);
}
