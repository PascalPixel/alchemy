#include "types.h"

#define FieldScene_RunScene3a4SequenceB Func_020009ec

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02004528();
void Func_02004534();
s32 Func_02004544();
void Func_02004558();
void Func_02004564();
void Func_02004586();
void Func_020045ae();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
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

void FieldScene_RunScene3a4SequenceB(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02004544, 0x323) != 0) {
        Call6(Func_02004534, 2, 0, 1, 1, 24, 80);
        Call6(Func_02004528, 2, 1, 24, 11, 1, 2);
        Call1(Func_02004586, 0x323);
    } else {
        Call6(Func_02004564, 0, 0, 1, 1, 24, 80);
        Call6(Func_02004558, 0, 1, 24, 11, 1, 2);
        Call1(Func_020045ae, 0x323);
    }
}
