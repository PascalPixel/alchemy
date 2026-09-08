#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02003c20();
void Func_02003c32();
void Func_02003c42();
void Func_02003c54();
void Func_02003c5c();
void Func_02003c60();
void Func_02003cc0();
void Func_02003cd0();
void Func_02003cd6();
void Func_02003cfc();
void Func_02003d02();
void Func_02003d06();
void Func_02003d14();
void Func_02003d1c();
void Func_02003d6c();
void Func_02003db4();
void Func_02003dca();
void Func_02003e00_a();
void Func_02003e00_b();
s32 Func_02003e14_a();
s32 Func_02003e14_b();
void Func_02003e16();
void Func_02003e2a();
void Func_02003e30();

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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void Func_0200416a();
s32 Func_02004150();
s32 Func_0200415a();
void Func_02004174();
void Func_02004158();
s32 Func_02004180();
s32 Func_0200418a();
void Func_0200419c();
s32 Func_0200419a();
s32 Func_020041a4();
void Func_020041be();
void Func_020041a2();
s32 Func_020041ca();
s32 Func_020041d4();
void Func_020041e6();
s32 Func_020041e4();
s32 Func_020041ee();
void Func_02004208();
void Func_020041ec();
s32 Func_02004214();
s32 Func_0200421e();
void Func_02004230();
s32 Func_0200422e();
s32 Func_02004238();
void Func_02004252();
void Func_02004236();
s32 Func_0200425e();
s32 Func_02004268();
void Func_0200427a();
void Func_02002b26();
void Func_02004342();
void Func_0200430c();
void Func_02004384();
void Func_02004390();
void Func_020042be();
void Func_020043ee();
void Func_02001bca();
void Func_020042d0();
void Func_020043fe();
void Func_02001d04();
void Func_020042e2();
void Func_02004416();
void Func_02001bf2();
void Func_020042f8();
void Func_02004426();
void Func_02001d2c();
void Func_0200430a();
void Func_0200443e();
void Func_02001c1a();
void Func_02004320();
void Func_0200444e();
void Func_02001d54();
void Func_02004332();
void Func_02004466();
void Func_02001c42();
void Func_02004348();
void Func_02004476();
void Func_02001d7c();
void Func_0200435a();
void Func_0200448e();
void Func_02001c6a();
void Func_02004370();
void Func_0200449e();
void Func_02001da4();
void Func_02004382();
void Func_020044b4();
void Func_02001c90();
void Func_02004396();
s32 Func_02004384_a();
void Func_02004430();
void Func_02003fe0();
void Func_02004418();
void Func_02003ff0();
void Func_020044ea();
void Func_020044f6();
void Func_020044c4();


void Func_02003eea();
void Func_0200277e();
void Func_02003f86();
void Func_02003fb2();
void Func_02003fc6();
void Func_02003f88();
void Func_0200401c();
void Func_02004028();
void Func_02003f56();
void Func_02003b8e();
void Func_0200408e();
void Func_020017d6();
void Func_02003f70();
void Func_0200409e();
void Func_02001910();
void Func_02003f82();
void Func_02003bbe();
u32 Func_02003f4c();
u32 Func_02003f5e();
u32 Func_02003f70_a();
u32 Func_02003f82_a();
s32 Func_02003f8e();
s32 Func_02003f98();
s32 Func_02003fa2();
s32 Func_02003fac();
void Func_02004134();
void Func_0200187c();
void Func_02004016();
void Func_02004144();
void Func_020019b6();
void Func_02004028_a();
void Func_0200415c();
void Func_020018a4();
void Func_0200403e();
void Func_0200416c();
void Func_020019de();
void Func_02004050();
void Func_02004184();
void Func_020018cc();
void Func_02004066();
void Func_02004194();
void Func_02001a06();
void Func_02004078();
void Func_020041ac();
void Func_020018f4();
void Func_0200408e_a();
void Func_020041bc();
void Func_02001a2e();
void Func_020040a0();
void Func_02004080();
void Func_0200412a();
void Func_02003cfa();
void Func_02004100();
void Func_02004126();
void Func_0200409c();
void Func_020040a2();
void Func_020040a8();
void Func_020040ae();
void Func_02004228();
void Func_02004234();
void Func_02004202();
extern s16 Data_0200ade4, Data_0200addc, Data_0200ade0, Data_0200adec;
extern s32 Data_0200ade8;
void UpdateStatueLight1(void);
void UpdateStatueLight2(void);
void UpdateStatueLight3(void);
void UpdateStatueLight4(void);

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

void FieldScene_PrepareStatueTransition(void)
{
    u32 i;
    s32 record;
    u8 *work;

    Call4(Func_02003d14, -1, -1, -1, 0);
    Call6(Func_02003c20, 30, 43, 32, 40, 8, 3);
    Call6(Func_02003c32, 30, 43, 33, 39, 8, 1);
    Call6(Func_02003c42, 30, 43, 36, 38, 3, 3);
    Call6(Func_02003c54, 14, 41, 32, 41, 8, 4);
    Call4(Func_02003d6c, 0x23e0000, -1, 0x9e0000, 0);
    Func_02003c60();
    Call3(Func_02003cfc, 16, 0x23e0000, 0x780000);
    Func_02003d06(0, 0, 0);
    Func_02003c5c(1);
    Call2(Func_02003db4, 0x2051cc, 1);
    Func_02003dca(20);
    Call1(Func_02003cc0, 0x201);
    Call1(Func_02003cd0, 0x200);
    Call1(Func_02003cd6, 0x202);
    work = *(u8 **)Data_03001ebc;
    *(s32 *)(((s32)work + 0x1c0)) = 0x100;
    *(s32 *)(((s32)work + 0x1c8)) = 32;
    Func_02003e00_a();
    ((void (*)())Func_02003e14_a)();
    Func_02003d02(40);
    Func_02003e30(171);
    Call2(Func_02003e00_b, 0x10005, 1);
    Func_02003e16(8);
    Func_02003d1c(32);
    Value2(Func_02003e14_b, 0x2051cc, 1);
    Func_02003e2a(24);
}

void FieldScene_RunClosingSequence(void)
{
    s32 i;
    u8 *work;
    Func_02003eea();
    Func_0200277e();
    Data_0200ade4 = 0;
    Data_0200addc = 0;
    Data_0200ade0 = 0;
    Data_0200adec = 0;
    Func_02003f86(4097);
    Call3(Func_02003fb2, 16, 16384, 20);
    Call3(Func_02003fc6, 16, 256, 0);
    Func_02003f88(16, 6, 30);
    Func_0200401c(37617664, -1, 11403264, 1);
    Func_02004028();
    Func_02003f56(30);
    Func_02003b8e(32784, 20);
    for (i = 0; i != 4; i++) {
        Func_0200408e(246);
        Func_020017d6();
        Func_02003f70(12);
        Func_0200409e(246);
        Func_02001910();
        Func_02003f82(12);
    }
    Func_02003bbe(32784, 6);
    Data_0200ade4 = ((Func_02003f4c() * 60) >> 16) + 20;
    Data_0200addc = ((Func_02003f5e() * 60) >> 16) + 20;
    Data_0200ade0 = ((Func_02003f70_a() * 60) >> 16) + 20;
    Data_0200adec = ((Func_02003f82_a() * 60) >> 16) + 20;
    Data_0200ade8 = 0;
    Value2(Func_02003f8e, (s32)UpdateStatueLight1, 3200);
    Value2(Func_02003f98, (s32)UpdateStatueLight2, 3200);
    Value2(Func_02003fa2, (s32)UpdateStatueLight3, 3200);
    Value2(Func_02003fac, (s32)UpdateStatueLight4, 3200);
    for (i = 0; i != 6; i++) {
        Func_02004134(246);
        Func_0200187c();
        Func_02004016(5);
        Func_02004144(246);
        Func_020019b6();
        Func_02004028_a(5);
    }
    for (i = 0; i != 8; i++) {
        Func_0200415c(246);
        Func_020018a4();
        Func_0200403e(4);
        Func_0200416c(246);
        Func_020019de();
        Func_02004050(4);
    }
    for (i = 0; i != 10; i++) {
        Func_02004184(246);
        Func_020018cc();
        Func_02004066(3);
        Func_02004194(246);
        Func_02001a06();
        Func_02004078(3);
    }
    for (i = 0; i != 12; i++) {
        Func_020041ac(246);
        Func_020018f4();
        Func_0200408e_a(2);
        Func_020041bc(246);
        Func_02001a2e();
        Func_020040a0(2);
    }
    Call6(Func_02004080, 45, 30, 34, 10, 4, 2);
    Func_0200412a(16, 6, 40);
    Func_02003cfa(32784, 6);
    Call3(Func_02004100, 16, 131072, 65536);
    Func_02004126(16, 576, 280);
    Func_0200409c((s32)UpdateStatueLight1);
    Func_020040a2((s32)UpdateStatueLight2);
    Func_020040a8((s32)UpdateStatueLight3);
    Func_020040ae((s32)UpdateStatueLight4);
    work = *(u8 **)Data_03001ebc;
    *(s32 *)(work + 0x1c0) = 0x100;
    *(s32 *)(work + 0x1c8) = 32;
    Func_02004228();
    Func_02004234();
    Func_02004202(4);
}

void FieldScene_RunFlaggedSequence(void)
{
    s32 base;
    u8 *work;
    s32 i6;
    s32 i7;
    s32 i8;
    s32 i9;
    s32 i10;

    Func_0200416a();
    if (Value1(Func_02004150, 2059) != 0 && Value1(Func_0200415a, 2086) != 0) {
        Call1(Func_02004174, 2086);
        Call6(Func_02004158, 45, 28, 34, 10, 2, 1);
    } else if (Value1(Func_02004180, 2059) != 0 && Value1(Func_0200418a, 2086) == 0) {
        Call1(Func_0200419c, 2086);
    }
    if (Value1(Func_0200419a, 2060) != 0 && Value1(Func_020041a4, 2087) != 0) {
        Call1(Func_020041be, 2087);
        Call6(Func_020041a2, 47, 28, 36, 10, 2, 1);
    } else if (Value1(Func_020041ca, 2060) != 0 && Value1(Func_020041d4, 2087) == 0) {
        Call1(Func_020041e6, 2087);
    }
    if (Value1(Func_020041e4, 2061) != 0 && Value1(Func_020041ee, 2088) != 0) {
        Call1(Func_02004208, 2088);
        Call6(Func_020041ec, 45, 29, 34, 11, 2, 1);
    } else if (Value1(Func_02004214, 2061) != 0 && Value1(Func_0200421e, 2088) == 0) {
        Call1(Func_02004230, 2088);
    }
    if (Value1(Func_0200422e, 2062) != 0 && Value1(Func_02004238, 2089) != 0) {
        Call1(Func_02004252, 2089);
        Call6(Func_02004236, 47, 29, 36, 11, 2, 1);
    } else if (Value1(Func_0200425e, 2062) != 0 && Value1(Func_02004268, 2089) == 0) {
        Call1(Func_0200427a, 2089);
    }
    Func_02002b26();
    Call3(Func_02004342, 16, 16384, 20);
    Func_0200430c(16, 6, 30);
    Func_02004384(37617664, -1, 11403264, 1);
    Func_02004390();
    Func_020042be(30);
    for (i6 = 0; i6 != 4; i6++) {
        Func_020043ee(246);
        Func_02001bca();
        Func_020042d0(12);
        Func_020043fe(246);
        Func_02001d04();
        Func_020042e2(12);
    }
    for (i7 = 0; i7 != 6; i7++) {
        Func_02004416(246);
        Func_02001bf2();
        Func_020042f8(8);
        Func_02004426(246);
        Func_02001d2c();
        Func_0200430a(8);
    }
    for (i8 = 0; i8 != 8; i8++) {
        Func_0200443e(246);
        Func_02001c1a();
        Func_02004320(6);
        Func_0200444e(246);
        Func_02001d54();
        Func_02004332(6);
    }
    for (i9 = 0; i9 != 10; i9++) {
        Func_02004466(246);
        Func_02001c42();
        Func_02004348(4);
        Func_02004476(246);
        Func_02001d7c();
        Func_0200435a(4);
    }
    for (i10 = 0; i10 != 12; i10++) {
        Func_0200448e(246);
        Func_02001c6a();
        Func_02004370(2);
        Func_0200449e(246);
        Func_02001da4();
        Func_02004382(2);
    }
    Func_020044b4(246);
    Func_02001c90();
    Func_02004396(6);
    if (Func_02004384_a(2082) == 0) {
        base = 32784;
        Func_02004430(4133);
        Func_02003fe0(base, 6);
        Func_02004418(16, 3);
        Func_02003ff0(base, 6);
    }
    work = *(u8 **)Data_03001ebc;
    *(s32 *)(work + 0x1c0) = 0x100;
    *(s32 *)(work + 0x1c8) = 32;
    Func_020044ea();
    Func_020044f6();
    Func_020044c4(5);
}
