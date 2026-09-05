#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001084();
void Func_020019a4();
s32 Func_020019b0();
void Func_020019b4();
void Func_020019ba();
void Func_020019ca();
void Func_020019d8();
void Func_020019ee();
void Func_020019fa();
void Func_02001a10();
void Func_02001a18();
void Func_02001a2e();
void Func_02001a44();
void Func_02001a4e();

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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
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

void FieldScene_RunScene3a7SequenceB(void)
{
    u32 i;
    s32 record;
    s32 v5;
    s32 v6;

    if (Value1(Func_020019b0, 0x9a8) == 0) {
        Call2(Func_020019b4, 0x1528, 1);
        Call1(Func_020019ca, 0x9a8);
        v5 = 27;
        v6 = 92;
        Func_02001a18(155);
        Call6(Func_020019a4, 107, 27, 1, 1, v6, v5);
        Func_020019fa(39);
        Call6(Func_020019ba, 108, 27, 1, 1, v6, v5);
        Func_02001a10(50);
        v6 = 25;
        Func_02001a4e(156);
        Call6(Func_020019d8, 1, 24, 1, 2, v6, v5);
        Func_02001a2e(40);
        Call6(Func_020019ee, 2, 24, 1, 2, v6, v5);
        Func_02001a44(40);
        Func_02001084();
    }
}
