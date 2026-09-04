#include "types.h"

#define FieldScene_RunScene3b9_02002964 Func_02002964

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200adac[];
extern u8 Data_03001ebc[];
void Func_02004604();
void Func_02004612();
void Func_02004620();
void Func_02004636();
void Func_020046b4();
void Func_0200555c();
void Func_0200558c();
void Func_020055ca();
void Func_020055d8();
void Func_020055e6();
void Func_0200560c();
void Func_02005624();
void Func_02005628();
void Func_02005636();
void Func_0200563c();
void Func_02005640();
void Func_02005644();
void Func_0200564a();
void Func_0200564e();
void Func_02005656();
void Func_0200565c();
s32 Func_0200565e();
void Func_02005676();
void Func_0200568a();
void Func_02005698();
void Func_020056a2();
void Func_020056b2();
void Func_020056b6();
void Func_020056c2();
void Func_020056c6();
void Func_020056cc();
void Func_020056d2();
void Func_020056d4();
void Func_020056dc();
void Func_020056de();
void Func_020056e4();
void Func_020056e8();
void Func_02005728();
void Func_0200572e();
void Func_0200579a();
void Func_020057b4();
void Func_020057fa();
void Func_02005818();
void Func_02005824();

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
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

void FieldScene_RunScene3b9_02002964(void)
{
    u32 i;
    s32 record;
    s32 base5_200adac;

    Func_0200555c();
    Call3(Func_020055ca, 1, 0x3180000, 0x880000);
    Call3(Func_020055d8, 2, 0x3380000, 0x880000);
    Call3(Func_020055e6, 3, 0x3280000, 0x980000);
    Func_020056b2();
    Func_020056c6();
    Func_0200558c(40);
    Func_02005624(8, 1);
    Func_0200560c(8, 3);
    Call1(Func_0200564a, 0x2138);
    Func_02004604(8);
    Func_02005640(9, 1);
    Func_02004612(9);
    Func_0200564e(10, 1);
    Func_02004620(10);
    Func_0200565c(11, 1);
    Func_02005644(11, 3);
    Func_02004636(11);
    Call3(Func_020056b6, 1, 0xe000, 0);
    Call3(Func_020056c2, 2, 0xa000, 20);
    Call3(Func_02005628, 1, 0x10000, 0x8000);
    Call3(Func_02005636, 2, 0x10000, 0x8000);
    Call3(Func_02005644, 3, 0x10000, 0x8000);
    base5_200adac = (s32)Data_0200adac;
    Func_02005656(1, base5_200adac);
    Value2(Func_0200565e, 2, base5_200adac);
    Func_02005676(3, base5_200adac);
    Func_0200563c(20);
    Func_020046b4(0, 0);
    Func_020056cc(0, 3);
    Func_020056d4(11, 3);
    Call3(Func_0200568a, 11, 0x10000, 0x8000);
    Call3(Func_02005698, 0, 0x10000, 0x8000);
    Func_020056e8(11, 2);
    Call3(Func_020056d2, 11, 0x33e, 152);
    Call3(Func_020056de, 11, 0x328, 164);
    Call3(Func_020056e4, 11, 0x328, 0x138);
    Func_020056a2(20);
    Call2(Func_0200579a, 0x6666, 0xccc);
    Call4(Func_020057b4, 0x3280000, -1, 0x1380000, 1);
    Call3(Func_02005728, 0, 0x328, 164);
    Call3(Func_0200572e, 0, 0x328, 0x138);
    Func_020056dc(60);
    Func_02005818();
    Func_02005824();
    Func_020057fa(64);
}
