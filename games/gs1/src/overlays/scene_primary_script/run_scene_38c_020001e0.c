#include "types.h"

#define FieldScene_RunScene38c_020001e0 Func_020001e0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020006fc();
void Func_02000714();
void Func_0200073a();
s32 Func_02000744();
void Func_02000746();
void Func_0200075c();
void Func_02000760();
s32 Func_02000764();
void Func_02000790();
void Func_020007b4();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

void FieldScene_RunScene38c_020001e0(void)
{
    u32 i;
    s32 record;

    Func_020006fc();
    Call1(Func_0200073a, 0x13f0);
    Func_0200075c(24, 0, 20);
    Func_02000746(24, 0, 0);
    Func_02000714(10);
    Value2(Func_02000764, 24, 0);
    if (Value2(Func_02000744, 0, 0) != 0) {
        bump_step(1);
    }
    Func_02000790(24, 0);
    Call3(Func_020007b4, 24, 0x4000, 10);
    Func_02000760();
}
