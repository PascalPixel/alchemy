#include "types.h"

#define FieldScene_RunScene3ce_02000244 Func_02000244

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02009684[];
extern u8 Data_03001ebc[];
void Func_020013fc();
void Func_0200149a();
void Func_020014a0();
void Func_020014a6();
void Func_020014ac();
void Func_020014c6();
s32 Func_020014ce();
s32 Func_020014d6();
void Func_020014de();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3ce_02000244(void)
{
    u32 i;
    s32 record;
    s32 base5_2009684;

    Call2(Func_020013fc, 0xc1a, 1);
    base5_2009684 = (s32)Data_02009684;
    Func_020014c6(0, *(volatile s32 *)base5_2009684);
    Value2(Func_020014ce, 1, *(volatile s32 *)base5_2009684);
    Value2(Func_020014d6, 3, *(volatile s32 *)base5_2009684);
    Func_020014de(2, *(volatile s32 *)base5_2009684);
    *(volatile s32 *)base5_2009684 += 10;
    Func_0200149a(0);
    Func_020014a0(1);
    Func_020014a6(3);
    Func_020014ac(2);
}
