#include "types.h"

#define FieldScene_RunScene37bSequenceD Func_020016a4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02003af8();
s32 Func_02003b16();
s32 Func_02003b20();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene37bSequenceD(void)
{
    s32 rec1;
    s32 rec2;
    s32 field;

    rec1 = Value1(Func_02003b16, 0);
    field = *(s32 *)(rec1 + 8) >> 20;
    rec2 = Value1(Func_02003b20, 0);
    if ((*(s32 *)(rec2 + 16) >> 20) == 7) {
        if ((u32)(field - 21) <= 1) {
            Call4(Func_02003af8, 2, 0x1600000, 0x700000, 255);
        }
    }
}
