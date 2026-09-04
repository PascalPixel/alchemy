#include "types.h"

#define SceneActor_MarkSlot10AndRunFlag202Step Func_020001b0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020002bc();
void Func_02001cc0();
void Func_02001ccc();
s32 Func_02001cde();
s32 Func_02001cf2();
void Func_02001d00();
void Func_02001e10();
void Func_02001e1a();

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

void SceneActor_MarkSlot10AndRunFlag202Step(void)
{
    u32 i;
    u8 *rec7;
    s32 record;

    rec7 = Value1(Func_02001cf2, 10);
    Call1(Func_02001ccc, 0x200);
    if ((s32)rec7 != 0) {
        Func_02001cc0((s32)rec7, 0);
        rec7[35] = 1;
    }
    if (Value1(Func_02001cde, 0x202) == 0) {
        Func_02001e10(157);
        Func_020002bc();
        Func_02001e1a(80);
        Call1(Func_02001d00, 0x202);
    }
}
