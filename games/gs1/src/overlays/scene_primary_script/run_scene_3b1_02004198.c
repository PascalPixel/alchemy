#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200e8e4[];
extern u8 Data_03001ebc[];
void Func_02007968();
void Func_02008a8e();
void Func_02008a98();
void Func_02008aa6();
void Func_020092e8();
void Func_0200a664();
s32 Func_0200a6ce();
void Func_0200a6d8();
s32 Func_0200a6e0();
s32 Func_0200a6e8();
void Func_0200a716();
void Func_0200a788();
void Func_0200a790();
void Func_0200a798();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

void FieldScene_RunScene3b1_02004198(void)
{
    u32 i;
    s32 record;
    s32 base5_200e8e4;

    Func_0200a664();
    Func_02008a8e(24, 1, 0);
    Func_02008a98(25, 1, 0);
    Func_02007968();
    Func_02008aa6(19, 11, 12);
    Func_0200a716(10, 6);
    Value2(Func_0200a6ce, 12, 0x200e840);
    base5_200e8e4 = (s32)Data_0200e8e4;
    Func_0200a6d8(36, base5_200e8e4);
    Value2(Func_0200a6e0, 37, base5_200e8e4);
    Value2(Func_0200a6e8, 38, base5_200e8e4);
    Func_0200a788(36, 3);
    Func_0200a790(37, 3);
    Func_0200a798(38, 3);
    Func_020092e8();
    Func_0200a6d8();
}
