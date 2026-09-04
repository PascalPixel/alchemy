#include "types.h"

#define FieldScene_RunScene3b1_020056dc Func_020056dc

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200e840[];
extern u8 Data_0200e8e4[];
extern u8 Data_03001ebc[];
void Func_02008ec4();
void Func_02009fea();
void Func_02009ff4();
s32 Func_0200a002();
s32 Func_0200a690();
s32 Func_0200a69a();
void Func_0200a84a();
void Func_0200bbc0();
void Func_0200bc22();
void Func_0200bc2c();
void Func_0200bc30();
void Func_0200bc3a();
void Func_0200bc4a();
s32 Func_0200bc52();
void Func_0200bc72();
void Func_0200bcf2();
void Func_0200bcfa();

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

void FieldScene_RunScene3b1_020056dc(void)
{
    u32 i;
    s32 rec2;
    s32 rec8;
    s32 record;
    s32 base5_200e840;
    s32 base5_200e8e4;

    rec8 = Value2(Func_0200a690, 0, 0);
    rec2 = Value2(Func_0200a69a, 1, 0);
    Func_0200bbc0();
    Func_02009fea(24, 1, 0);
    Func_02009ff4(25, 3, 0);
    Func_02008ec4();
    Value3(Func_0200a002, 19, rec8, rec2);
    Func_0200bc72(10, 6);
    base5_200e840 = (s32)Data_0200e840;
    Func_0200bc2c(rec8, base5_200e840);
    Func_0200bc22(11);
    Func_0200bc3a(rec2, base5_200e840);
    Func_0200bc30(12);
    base5_200e8e4 = (s32)Data_0200e8e4;
    Func_0200bc4a(36, base5_200e8e4);
    Value2(Func_0200bc52, 37, base5_200e8e4);
    Func_0200bcf2(36, 3);
    Func_0200bcfa(37, 3);
    Func_0200a84a();
    Func_0200bc3a();
}
