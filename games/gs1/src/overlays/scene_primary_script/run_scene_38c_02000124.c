#include "types.h"

#define FieldScene_RunScene38c_02000124 Func_02000124

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02000648();
s32 Func_0200064a();
s32 Func_02000662();
void Func_02000674();
void Func_020006a4();
void Func_020006a6();
void Func_020006ac();
void Func_020006ca();
void Func_020006d0();
s32 Func_020006dc();
void Func_020006e8();
void Func_020006f0();

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

void FieldScene_RunScene38c_02000124(void)
{
    u32 i;
    s32 record;
    u8 *p5;

    record = Value1(Func_02000662, 0);
    p5 = *(volatile u16 *)(record + 6);
    Func_02000648();
    if ((u32)((s32)p5 + -0xa001) <= 0x3ffe) {
        Func_020006ca(8, 17);
    } else {
        if (Value1(Func_0200064a, 0x845) == 0) {
            Call1(Func_020006a4, 0x13e5);
            Func_020006a6(17, 0, 0);
            Func_02000674(10);
            Value2(Func_020006dc, 17, 0);
            Call3(Func_020006f0, 17, 0x3000, 10);
        } else {
            Call1(Func_020006d0, 0x16f7);
            Func_020006e8(17, 0);
        }
    }
    Func_020006ac();
}
