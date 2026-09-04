#include "types.h"

#define FieldScene_RunScene38c_02000250 Func_02000250

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_0200076c();
void Func_0200077a();
s32 Func_020007aa();
void Func_020007ac();
void Func_020007c6();
s32 Func_020007ca();
void Func_020007f6();
void Func_0200081a();

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

void FieldScene_RunScene38c_02000250(void)
{
    u32 i;
    s32 record;

    Func_0200076c();
    Call1(Func_020007aa, 0x13f6);
    Func_020007ac(27, 0, 0);
    Func_0200077a(10);
    Value2(Func_020007ca, 27, 0);
    if (Value2(Func_020007aa, 0, 0) != 0) {
        bump_step(1);
    }
    Func_020007f6(27, 0);
    Call3(Func_0200081a, 27, 0x4000, 10);
    Func_020007c6();
}
