#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_0200080c();
s32 Func_02001f6c();
s32 Func_02001f76();
void Func_02001f7a();
void Func_02001f9a();
void Func_02001fa8();
void Func_020020b0();

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

void FieldScene_RunGuardedLine1528Scene(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02001f6c, 0x200) == 0) {
        if (Value1(Func_02001f76, 0x202) == 0) {
            Call2(Func_02001f7a, 0x1528, 1);
            Func_020020b0(157);
            Func_0200080c();
            Call1(Func_02001f9a, 0x202);
            Call1(Func_02001fa8, 0x201);
        }
    }
}
