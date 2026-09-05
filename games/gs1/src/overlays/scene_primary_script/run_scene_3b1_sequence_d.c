#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_020062c6();
s32 Func_02007eae();
void Func_02007eb6();
void Func_02007ede();
void Func_02007f1a();
void Func_02007f28();
void Func_02007f6c();
void Func_02007ff4();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
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

void FieldScene_RunScene3b1SequenceD(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02007eae, 0x301) != 0) {
        Func_02007ede();
        Func_02007ff4(8);
        Call3(Func_02007eb6, 0x1e48, 1, 8);
        Call3(Func_02007f28, 0, 0x19999, 0xcccc);
        Call3(Func_02007f6c, 0, 0x198, 134);
        Value2(Func_020062c6, 0, 0x4000);
        Func_02007f1a();
    }
}
