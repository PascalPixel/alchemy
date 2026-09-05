#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02004bde();
void Func_02004c0a();
s32 Func_02004c14();
void Func_02004c16();
void Func_02004c42();
s32 Func_02004c56();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
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

void FieldScene_RunScene3b3_020020f0(s32 a0)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value0(Func_02004c14);
    Func_02004c0a();
    *(s32 *)(rec7 + 108) = 0x200a0b9;
    Call6(Func_02004bde, 20, 14, 1, 1, (*(s32 *)(rec7 + 8) >> 20), (*(s32 *)(rec7 + 16) >> 20));
    Func_02004c16((a0 + 0x1f5));
    Value2(Func_02004c56, a0, 0x200ad64);
    Func_02004c42();
}
