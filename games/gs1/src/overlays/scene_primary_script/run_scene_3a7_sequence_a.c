#include "types.h"

#define FieldScene_RunScene3a7SequenceA Func_020003e0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02000722();
void Func_0200079a();
s32 Func_020014e6();
void Func_02001500();
void Func_0200154e();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

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

void FieldScene_RunScene3a7SequenceA(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_020014e6, 0x9a9) == 0) {
        if (Value0(Func_02000722) != 0) {
            Call1(Func_02001500, 0x9a9);
            Func_0200154e(80);
            Func_0200079a();
        }
    }
}
