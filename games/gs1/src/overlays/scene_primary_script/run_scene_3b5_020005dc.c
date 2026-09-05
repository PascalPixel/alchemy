#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_0200132a();
void Func_0200133c();
void Func_0200138a();
void Func_02001396();
void Func_020013d2();
void Func_020013ea();
void Func_02001402();
void Func_02001412();
void Func_0200141a();
void Func_02001432();
void Func_02001452();

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

void FieldScene_RunScene3b5_020005dc(void)
{
    u32 i;
    s32 record;

    Func_0200133c();
    if (Value1(Func_0200132a, 0x8bf) == 0) {
        Call1(Func_0200133c, 0x8bf);
        Call1(Func_020013ea, 0x2368);
        Func_02001402(19, 0);
        Func_02001452(233, 3);
        Func_02001412(19, 0);
        Func_020013d2(0, 1);
        Func_0200138a(233, 0);
    } else {
        Call1(Func_0200141a, 0x236a);
        Func_02001432(19, 0);
    }
    Func_02001396();
}
