#include "types.h"

#define FieldScene_RunScene38b_02000240 Func_02000240

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_020012e8();
void Func_020012fc();
void Func_02001312();
void Func_02001336();
void Func_0200137a();
void Func_020013a4();

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

void FieldScene_RunScene38b_02000240(void)
{
    u32 i;
    s32 record;

    Func_020012fc();
    Call1(Func_0200137a, 0x13ae);
    if (Value1(Func_020012e8, 0x301) != 0) {
        bump_step(1);
    }
    Func_020013a4(9, 0);
    Call1(Func_02001312, 0x301);
    Func_02001336();
}
