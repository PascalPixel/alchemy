#include "types.h"

#define FieldScene_RunScene383SequenceB Func_02001ba0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00001299[];
void Func_02006290();
void Func_020062bc();
void Func_020062ec();
void Func_020062f0();
void Func_020062fe();
void Func_0200631e();
void Func_02006348();
void Func_0200634e();
void Func_02006352();
void Func_02006378();
void Func_02006388();
void Func_020063b8();
void Func_020063fa();
void Func_02006838();
void Func_020068ea();
void Func_020068fa();
s32 Func_02006924();
s32 Func_0200692c();
void Func_02006962();
void Func_0200696c();
void Func_02006980();
void Func_0200698a();
void Func_0200698c();
void Func_02006996();
void Func_020069a0();
void Func_020069dc();
void Func_020069fe();
void Func_02006a02();
void Func_02006a10();
void Func_02006a12();
void Func_02006a28();
s32 Func_02006a50();
void Func_02006a60();
void Func_02006a7e();
void Func_02006a88();
void Func_02006a8c();
void Func_02006a9c();
void Func_02006aa8();
void Func_02006aaa();
void Func_02006aae();
void Func_02006ac0();
void Func_02006ae0();
s32 Func_02006af0();
void Func_02006b0c();
void Func_02006b12();
void Func_02006b14();
void Func_02006b3c();
void Func_02006b5a();
void Func_02006b76();
void Func_02006b80();
void Func_02006b88();
void Func_02006b8a();
void Func_02006b9c();
void Func_02006b9e();
void Func_02006ba8();
void Func_02006bac();
void Func_02006bb6();
void Func_02006bc4();
void Func_02006bca();
void Func_02006bd2();
void Func_02006c3c();
void Func_02006c48();

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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene383SequenceB(void)
{
    u32 i;
    s32 rec7;
    s32 rec8;
    s32 record;
    s32 base5_1299;

    rec7 = Value1(Func_02006924, 24);
    rec8 = Value1(Func_0200692c, 25);
    Func_020068fa();
    Call1(Func_02006838, 0x200aba1);
    Call1(Func_020068ea, 0x300);
    if (*(s16 *)(rec7 + 100) <= 3) {
        Func_02006962(24, 0x200d678);
    } else {
        Func_0200696c(24, 0x200d650);
    }
    if (*(s16 *)(rec8 + 100) <= 2) {
        Func_02006980(25, 0x200d768);
    } else {
        Func_0200698a(25, 0x200d650);
    }
    Call3(Func_0200698c, 0, 0xcccc, 0x6666);
    Call3(Func_02006996, 1, 0xcccc, 0x6666);
    Call3(Func_020069a0, 2, 0xcccc, 0x6666);
    Call3(Func_020069dc, 0, 248, 0x2d8);
    Call3(Func_02006a02, 2, 0xf80000, 0x2d80000);
    Call3(Func_02006a10, 1, 0xf80000, 0x2d80000);
    Call3(Func_020069fe, 2, 0x108, 0x2e8);
    Call3(Func_02006a12, 1, 232, 0x2e8);
    Func_02006a28(2);
    Call3(Func_02006a9c, 0, 0x4000, 0);
    Func_02006a7e(1, 0, 0);
    Func_02006290(2, 0, 30);
    Func_02006a88(2, 1);
    base5_1299 = (s32)Data_00001299;
    Func_02006aa8(base5_1299);
    Func_02006ac0(2, 0);
    Func_02006aaa(0, 2, 0);
    Func_020062bc(1, 2, 20);
    Func_02006a8c(0, 3);
    Func_020062fe(1, 3, 20);
    Func_020062f0(0, 1, 10);
    Value2(Func_02006af0, 1, 0);
    if (Value2(Func_02006a50, 0, 0) != 0) {
        bump_step(1);
    }
    Func_020062ec(1, 30);
    Func_02006b12((base5_1299 + 4));
    Func_02006b0c(0, 2, 0);
    Func_0200631e(1, 2, 50);
    Call3(Func_02006b5a, 2, 0x100, 0);
    Func_02006a60(60);
    Func_02006352(0, 1, 50);
    Func_02006b3c(0, 2, 0);
    Func_0200634e(1, 2, 30);
    Func_02006388(2, 3, 10);
    Func_02006348(2, 20);
    Call3(Func_02006b9c, 0, 0x102, 0);
    Call3(Func_02006ba8, 1, 0x102, 0);
    Func_02006aae(60);
    Func_020063b8(2, 3, 20);
    Func_02006378(2, 30);
    Call1(Func_02006b9e, 0x129f);
    Func_02006bb6(1, 0);
    Call3(Func_02006bca, 0, 0x4000, 0);
    Func_02006ae0(40);
    Func_02006b80(0, 3);
    Func_02006b88(1, 3);
    Func_020063fa(2, 3, 50);
    Call3(Func_02006b76, 2, 248, 0x2d8);
    Call3(Func_02006b8a, 1, 248, 0x2d8);
    Func_02006bac(1, 0, 0);
    Func_02006bb6(2, 0, 0);
    Call3(Func_02006bc4, 24, 0x680000, 0x2b80000);
    Call3(Func_02006bd2, 25, 0x780000, 0x2b80000);
    Func_02006c3c(24, 0, 0);
    Call3(Func_02006c48, 25, 0x8000, 0);
    Call6(Func_02006b14, 14, 50, 3, 1, 14, 44);
    Func_02006b80();
}
