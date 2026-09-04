#include "types.h"

#define FieldScene_RunScene3adSequenceA Func_0200033c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02000574();
s32 Func_02001e46();
s32 Func_02001e52();
void Func_02001e56();
void Func_02001e76();
void Func_02001e84();
void Func_02001f8c();

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

void FieldScene_RunScene3adSequenceA(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02001e46, 0x201) == 0) {
        if (Value1(Func_02001e52, 0x200) == 0) {
            Call2(Func_02001e56, 0x1528, 1);
            Func_02001f8c(157);
            Func_02000574();
            Call1(Func_02001e76, 0x201);
            Call1(Func_02001e84, 0x202);
        }
    }
}
