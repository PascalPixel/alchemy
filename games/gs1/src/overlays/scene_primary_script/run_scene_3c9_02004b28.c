#include "types.h"

#define FieldScene_RunScene3c9_02004b28 Func_02004b28

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020053c8();
void Func_0200542c();
void Func_02005464();
void Func_0200a806();
void Func_0200a8a4();
void Func_0200a8d4();
void Func_0200a8ee();
void Func_0200a922();
void Func_0200a95c();
void Func_0200a96e();
void Func_0200a986();
void Func_0200a992();
void Func_0200a996();
void Func_0200a99e();
void Func_0200a9aa();
void Func_0200a9b6();
void Func_0200a9be();
void Func_0200a9ce();
void Func_0200a9e8();
void Func_0200aa06();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3c9_02004b28(void)
{
    u32 i;
    s32 record;

    Call1(Func_0200a922, 0x2829);
    Func_020053c8(21);
    Func_0200aa06(62);
    Call3(Func_0200a806, 0x10000, 0x10000, 0x10000);
    Call2(Func_0200a986, 0x4cccc, 0x9999);
    Call2(Func_0200a992, 0x40000, 0x8000);
    Call4(Func_0200a9aa, 0xc00000, -0x400000, 0xee0000, 1);
    Func_0200a9b6();
    Func_0200a8a4(40);
    Func_0200a95c(21, 1);
    Call3(Func_0200a996, 0x2015, 0, 40);
    Func_0200a96e(6, 3);
    Func_0200542c(6);
    Call2(Func_0200a9ce, 21, 0x102);
    Func_0200a8d4(60);
    Call3(Func_0200a9be, 0x2015, 0, 80);
    Call2(Func_0200a9e8, 6, 0x102);
    Func_0200a8ee(40);
    Func_0200a99e(6, 2);
    Func_02005464(6);
}
