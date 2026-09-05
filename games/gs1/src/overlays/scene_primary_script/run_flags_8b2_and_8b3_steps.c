#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_020021e6();
s32 Func_020021f0();
void Func_02002200();
void Func_02002202();
void Func_02002208();
void Func_02002238();
void Func_020022c4();
void Func_02002306();

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

void FieldScene_RunFlags8B2And8B3Steps(void)
{
    u32 i;
    s32 record;

    Func_02002200();
    if (Value1(Func_020021e6, 0x8b2) == 0) {
        if (Value1(Func_020021f0, 0x8b3) == 0) {
            Call1(Func_02002202, 0x8b3);
            Call1(Func_02002208, 0x8b2);
        }
    }
    Func_02002306(123);
    Func_020022c4(3);
    Func_02002238();
}
