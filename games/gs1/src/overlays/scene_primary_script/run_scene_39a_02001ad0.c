#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02003e10();
void Func_02003e3e();
void Func_02003e54();
s32 Func_02003e58();
void Func_02003e78();
void Func_02003e7a();
void Func_02003e88();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene39a_02001ad0(s32 a0, s32 a1)
{
    u32 i;
    s32 record;

    Func_02003e10();
    Call3(Func_02003e3e, 0, 0x28000, 0x14000);
    Value3(Func_02003e58, 0, a0, a1);
    Func_02003e7a(0, 4, 0);
    Func_02003e7a(0, 7);
    Func_02003e78(0);
    Func_02003e88(0, 6);
    Func_02003e54();
}
