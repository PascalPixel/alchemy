#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02000600();
void Func_020008c6();
s32 Func_020008f6();
s32 Func_02001596();
s32 Func_020015a8();
void Func_020015c4();
void Func_02001602();

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

void FieldScene_RunGuardedStep9AAAfterSetup(void)
{
    u32 i;
    s32 record;

    Func_02000600();
    if (Value1(Func_02001596, 0x9aa) == 0) {
        if (Value0(Func_020008f6) != 0) {
            if (Value1(Func_020015a8, 0x207) == 0) {
                Func_02001602(80);
                Func_020008c6();
                Call1(Func_020015c4, 0x9aa);
            }
        }
    }
}
