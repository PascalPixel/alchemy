#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02003c10();
void Func_02003c1a();
void Func_02003c20();
void Func_02003c26();
void Func_02003c3a();
void Func_02003c44();
void Func_02003c5e();
void Func_02003c60();
void Func_02003c66();
void Func_02003ca4();
void Func_02003cbc();
void Func_02003d02();
void Func_02003d04();
void Func_02003d14();
void Func_02003d3c();
void Func_02003d46();
void Func_02003d50();
void Func_02003d5e();
void Func_02003d64();
void Func_02003d82();
void Func_02003d84();
void Func_02003d86();
void Func_02003da2();
void Func_02003da4();
void Func_02003da8();
void Func_02003daa();
void Func_02003db2();
void Func_02003dc8();
void Func_02003dda();
void Func_02003dea();
void Func_02003dee();
void Func_02003df2();
void Func_02003e00();
void Func_02003e06();
s32 Func_02003e0a();
void Func_02003e14();
void Func_02003e18();
void Func_02003e36();
void Func_02003e40();
void Func_02003e48();

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

void FieldScene_RunScene38d_0200179c(void)
{
    u32 i;
    s32 record;

    Func_02003c10();
    Func_02003d3c();
    Func_02003d50();
    Call3(Func_02003d14, 19, 0x3000, 0);
    Call3(Func_02003c5e, 0, 0x9999, 0x4ccc);
    Call3(Func_02003ca4, 0, 0x100, 0x294);
    Func_02003c3a(20);
    Call4(Func_02003d64, -1, -1, -1, 0);
    Call1(Func_02003c44, 0x200);
    Func_02003daa(188);
    Func_02003c20(1);
    Func_02003c26(2);
    Call3(Func_02003d04, 19, 0x1000000, 0x2780000);
    Func_02003c1a(1);
    Call3(Func_02003cbc, 19, 0x9999, 0x4ccc);
    Call3(Func_02003d02, 19, 0x100, 0x284);
    Func_02003c60(1);
    Func_02003c66(2);
    Func_02003ca4(20);
    Func_02003d64(19, 2);
    Call1(Func_02003d82, 0x145e);
    Func_02003da4(19, 0, 10);
    Call3(Func_02003dc8, 0, 0x100, 40);
    Call3(Func_02003d46, 0, 0x108, 0x294);
    Call3(Func_02003dda, 0, 0x8000, 0);
    Call3(Func_02003d5e, 19, 248, 0x294);
    Call3(Func_02003df2, 19, 0x1000, 40);
    Func_02003da2(19, 4);
    Func_02003dea(19, 0);
    Func_02003db2(19, 3);
    Value2(Func_02003e0a, 19, 0);
    Func_02003dda(19, 2);
    Func_02003e14(19, 0, 10);
    Call3(Func_02003e36, 0, 0x101, 60);
    Call2(Func_02003e48, 19, 0x102);
    Func_02003d46(60);
    Func_02003e06(19, 1);
    Func_02003e40(19, 0, 10);
    Func_02003e00(19, 3);
    Func_02003e48(19, 0);
    Call3(Func_02003daa, 19, 0xcccc, 0x6666);
    Call3(Func_02003dee, 19, 248, 0x304);
    Func_02003e18(19, 0, 0);
    Call1(Func_02003d86, 0x12f);
    Call1(Func_02003d84, 0x84f);
    Func_02003da8();
}
