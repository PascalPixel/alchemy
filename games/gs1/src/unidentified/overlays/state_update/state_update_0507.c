#include "types.h"

#define Func_020015b4 Func_020015b4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02005190();
void Func_020051b0();
s32 Func_020051e6();

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

void Func_020015b4(void)
{
    u32 i;
    s32 record;
    u8 *p5;

    if (Value1(Func_02005190, 0x200) == 0) {
        p5 = *(volatile s32 *)0x03001ee0;
        record = Value1(Func_020051e6, 0);
        *(volatile s32 *)((s32)p5 + 24) = record;
        Call1(Func_020051b0, 0x200);
    }
}
