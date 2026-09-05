#include "types.h"

#define FieldScene_RunClosingSequence Func_02001624

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02003a78();
void Func_02003a88();
s32 Func_02003a96();
s32 Func_02003aa0();

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
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunClosingSequence(void)
{
    s32 first;
    s32 kind;
    s32 second;

    first = Value1(Func_02003a96, 0);
    kind = *(volatile s32 *)(first + 8) >> 20;
    second = Value1(Func_02003aa0, 0);
    if ((*(volatile s32 *)(second + 16) >> 20) == 8) {
        if ((u32)(kind - 17) <= 1) {
            Call4(Func_02003a78, 2, 0x1100000, 0x800000, 255);
            Call4(Func_02003a88, 2, 0x1200000, 0x800000, 255);
        }
    }
}
