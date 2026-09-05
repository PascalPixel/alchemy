#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_0200183c();
s32 Func_02001846();
void Func_02001866();
void Func_02001882();
void Func_02001918();
s32 Func_02001928();
void Func_02001954();

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

void FieldScene_RunScene3ae_020002dc(void)
{
    u32 i;
    s32 record;

    Func_02001882(0);
    Func_02001866();
    if (Value1(Func_0200183c, 0x8a7) != 0) {
        if (Value1(Func_02001846, 0x8a9) != 0) {
            Call1(Func_02001918, 0x1d23);
            Value2(Func_02001928, 12, 0);
            Call3(Func_02001954, 12, 0x4000, 0);
        }
    }
}
