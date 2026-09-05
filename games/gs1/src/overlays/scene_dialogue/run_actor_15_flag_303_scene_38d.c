#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_020026ec();
void Func_02002700();
void Func_0200270c();
void Func_02002730();
void Func_020027ce();
void Func_020027de();
void Func_020027f6();

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

void SceneDialogue_RunActor15Flag303Scene(void)
{
    u32 i;
    s32 record;

    Func_02002700();
    Call1(Func_020027ce, 0x1756);
    if (Value1(Func_020026ec, 0x303) != 0) {
        Call1(Func_020027de, 0x176c);
    }
    Func_020027f6(15, 0);
    Call1(Func_0200270c, 0x303);
    Func_02002730();
}
