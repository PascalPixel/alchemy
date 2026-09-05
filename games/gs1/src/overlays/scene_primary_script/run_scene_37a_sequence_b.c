#include "types.h"

#define FieldScene_RunScene37aSequenceB Func_02000108

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02002ac4();
void Func_02002aec();
void Func_02002afa();
void Func_02002b00();
void Func_02002b1c();
void Func_02002bde();
void Func_02002bf4();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene37aSequenceB(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02002ac4, 0x200) == 0) {
        Func_02002aec();
        Call2(Func_02002bde, 0x10000, 1);
        Func_02002bf4(20);
        Call1(Func_02002aec, 0x200);
        Call1(Func_02002afa, 0x201);
        Call1(Func_02002b00, 0x202);
        Func_02002b1c();
    }
}
