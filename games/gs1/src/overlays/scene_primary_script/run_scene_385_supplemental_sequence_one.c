#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_02000804

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000002[];
extern u8 Data_00001cc0[];
extern u8 Data_03001ebc[];
void Func_02000ce2();
void Func_02000dce();
s32 Func_020018de();
void Func_02001900();
s32 Func_0200190a();
void Func_02001910();
void Func_0200193c();
u8 *Func_02001952();
void Func_02001990();

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

void FieldScene_RunSupplementalSequenceOne(void)
{
    u32 i;
    u8 *record;

    {
        u16 *flags = (u16 *)(Func_0200190a(14) + 100);
        u16 bits = (u16)(s32)Data_00000002;

        *flags |= bits;
    }
    Func_02001900();
    if (Value1(Func_020018de, 0x307) != 0) {
        Func_02001990((s32)Data_00001cc0);
        Func_02000ce2(14);
    } else {
        Func_02000dce();
        Call1(Func_02001910, 0x307);
    }
    Func_0200193c();
    {
        u8 *record = Func_02001952(14);
        s32 shown = 1;
    
        *(volatile u16 *)((s32)record + 100) = shown;
    }
}
