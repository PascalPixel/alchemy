#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200e840[];
extern u8 Data_0200e8e4[];
extern u8 Data_03001ebc[];
void Func_02008842();
void Func_02009968();
void Func_02009972();
s32 Func_02009980();
s32 Func_0200a018();
void Func_0200a1b2();
void Func_0200b53e();
void Func_0200b5a0();
void Func_0200b5a2();
void Func_0200b5aa();
s32 Func_0200b5b8();
void Func_0200b5c2();
void Func_0200b5ca();
void Func_0200b5f0();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3b1_02005068(void)
{
    u32 i;
    s32 rec8;
    s32 record;
    s32 base5_200e840;
    s32 base5_200e8e4;

    rec8 = Value2(Func_0200a018, 0, 0);
    Func_0200b53e();
    Func_02009968(24, 1, 0);
    Func_02009972(25, 2, 0);
    Func_02008842();
    Value3(Func_02009980, 19, rec8, 12);
    Func_0200b5f0(10, 6);
    base5_200e840 = (s32)Data_0200e840;
    Func_0200b5aa(rec8, base5_200e840);
    Func_0200b5a0(11);
    Value2(Func_0200b5b8, 12, base5_200e840);
    base5_200e8e4 = (s32)Data_0200e8e4;
    Func_0200b5c2(36, base5_200e8e4);
    Func_0200b5ca(37, base5_200e8e4);
    Func_0200a1b2();
    Func_0200b5a2();
}
