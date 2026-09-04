#include "types.h"

#define Func_02000634 Func_02000634

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02002cb2();
void Func_02002cd2();
void Func_02002cd4();
void Func_02002d06();
void Func_02002dac();
void Func_02002db4();
void Func_02002dd2();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void Func_02000634(void)
{
    u32 i;
    s32 record;

    Func_02002cd4();
    if (Value1(Func_02002cb2, 0x302) != 0) {
        Call1(Func_02002dac, 0x1be4);
    } else {
        Call1(Func_02002db4, 0x1be3);
        Call1(Func_02002cd2, 0x302);
    }
    Func_02002dd2(11, 0);
    Func_02002d06();
}
