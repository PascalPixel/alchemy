#include "types.h"

#define RunEventScript01 Func_02002fd4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200d17c[];
extern u8 Data_03001ebc[];
void Func_020076ba();
void Func_0200770c();
void Func_02007726();
void Func_020077a2();
void Func_020077e2();
void Func_020077e6();
void Func_020077f6();
void Func_02007824();
void Func_02007848();
void Func_02007860();
void Func_02007870();
void Func_02007890();
void Func_020078b2();
void Func_020078b6();
void Func_020078bc();
void Func_020078d2();
void Func_020078d8();
void Func_020078ec();
void Func_020078f6();
void Func_02007912();
void Func_02007936();
void Func_02007950();
void Func_02007960();
void Func_02007980();
void Func_02007984();
void Func_020079ae();
void Func_020079b8();
void Func_020079fe();
void Func_02007a2a();
void Func_02007a56();
void Func_02007a90();
void Func_02007aa6();
void Func_02007ab0();
void Func_02007abc();
void Func_02007abe();
void Func_02007b18();
void Func_02007b78();
void Func_02007b90();
void Func_02007b96();
void Func_02007b98();
void Func_02007ba0();
void Func_02007bc0();
void Func_02007bd0();
void Func_02007bda();
void Func_02007bf0();
void Func_02007c00();
void Func_02007c04();
void Func_02007c12();
void Func_02007c26();
void Func_02007c3c();
void Func_02007c4a();
void Func_02007c66();
void Func_02007c6a();
void Func_02007c6c();
void Func_02007c7c();
void Func_02007c8e();
void Func_02007cb6();
void Func_02007cbc();
void Func_02007cf0();
void Func_02007d02();
void Func_02007d0e();
void Func_02007d1a();
void Func_02007d24();
s32 Func_02007d56();
s32 Func_02007d8c();
s32 Func_02007d98();
s32 Func_02007da4();
void Func_02007dae();
void Func_02007dba();
s32 Func_02007dc8();
void Func_02007dd6();
void Func_02007df4();
void Func_02007e0a();
void Func_02007e12();
void Func_02007e1a();
void Func_02007e38();
void Func_02007e42();
void Func_02007e44();
void Func_02007e46();
void Func_02007e50();
void Func_02007e5e();
void Func_02007e66();
void Func_02007e82();
void Func_02007e90();
void Func_02007eca();
void Func_02007ece();
void Func_02007ed6();
void Func_02007ed8();
void Func_02007ee2();
void Func_02007ef4();
void Func_02007ef8();
void Func_02007efe();
void Func_02007f0c();
void Func_02007f24();
void Func_02007f2e();
void Func_02007f3e();
void Func_02007f42();
void Func_02007f5c();
void Func_02007f60();
void Func_02007f70();
void Func_02007f82();
void Func_02007f8e();
void Func_02007fa8();
void Func_02007fc2();
void Func_02007fc4();
void Func_02007fce();
void Func_02007fd0();
void Func_02007fd6();
void Func_02007fda();
void Func_02007fe4();
void Func_02008010();
void Func_02008024();
void Func_0200802c();
void Func_0200803c();
void Func_02008044();
void Func_02008054();
void Func_0200805e();
void Func_02008070();
void Func_02008096();
void Func_020080a6();
void Func_020080ca();
void Func_020080ee();
s32 Func_020080f8();
void Func_02008126();
void Func_0200813e();
void Func_02008140();
void Func_02008150();
void Func_02008174();
void Func_02008186();
s32 Func_02008198();
void Func_0200819c();
void Func_020081a2();
void Func_020081a8();
void Func_020081b2();
void Func_020081d0();
void Func_020081fc();
void Func_02008218();
void Func_0200821e();
void Func_0200822c();
void Func_0200822e();
void Func_02008236();
void Func_02008238();
void Func_0200823e();
void Func_02008256();
void Func_02008260();
void Func_0200826a();
void Func_0200826e();
void Func_02008280();
void Func_020082a2();
void Func_020082ac();
void Func_020082b6();
void Func_020082ba();
void Func_020082c0();
void Func_020082dc();
void Func_020082e6();
void Func_020082f0();
void Func_02008300();
void Func_02008302();
void Func_02008306();
void Func_0200831c();
void Func_02008366();
void Func_0200837a();
void Func_02008384();
void Func_020083aa();
void Func_020083ba();
void Func_020083d8();
void Func_020083e4();
void Func_020083f4();
void Func_020083fa();
void Func_02008416();
void Func_0200841a();
void Func_0200842a();
void Func_02008454();
void Func_02008464();
void Func_02008476();
void Func_0200847e();
void Func_02008574();
void Func_02008580();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void RunEventScript01(void)
{
    u32 i;
    s32 record;
    u8 *work;
    s32 base5_200d17c;
    u8 *p5;

    record = Value1(Func_02007d56, 12);
    p5 = *(volatile s32 *)(record + 80);
    Func_02007d24();
    Call3(Func_02007dba, 10, 0x3180000, 0x1a00000);
    Call3(Func_02007dc8, 11, 0x3200000, 0x1900000);
    Call3(Func_02007dd6, 12, 0x3080000, 0x1880000);
    record = Func_02007d8c(10);
    Func_02007d02(record, 0);
    record = Func_02007d98(11);
    Func_02007d0e(record, 0);
    record = Func_02007da4(12);
    Func_02007d1a(record, 0);
    Func_02007e0a(10, 9);
    Func_02007e12(11, 9);
    Func_02007e1a(12, 9);
    *(u8 *)(Func_02007dc8(12) + 35) &= 254;
    p5[9] |= 12;
    base5_200d17c = (s32)Data_0200d17c;
    Func_02007df4(10, base5_200d17c);
    Call3(Func_02007e42, 0, 0x3180000, 0x1b80000);
    Call3(Func_02007e50, 1, 0x3280000, 0x1b00000);
    Call3(Func_02007e5e, 2, 0x3080000, 0x1b80000);
    Call3(Func_02007eca, 0, 0xc000, 0);
    Call3(Func_02007ed6, 1, 0xb000, 0);
    Call3(Func_02007ee2, 2, 0xb000, 0);
    Call3(Func_02007e44, 8, 0xcccc, 0x6666);
    Call3(Func_02007ef8, 8, 0xb000, 0);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x209;
    Func_02007f2e(0, 0);
    Func_02007f42();
    Func_02007dae();
    Func_020076ba();
    Func_02007e82(11, base5_200d17c);
    Func_02007e38(30);
    Func_02007e90(12, base5_200d17c);
    Func_02007e46(30);
    Call1(Func_02007f24, 0x12e4);
    Func_0200770c(10, 20);
    Call3(Func_02007f60, 8, 0x102, 0);
    Func_02007e66(60);
    Func_02007726(8, 20);
    Call3(Func_02007ef4, 8, 0x328, 0x1c8);
    Call3(Func_02007ece, 1, 0xcccc, 0x6666);
    Call3(Func_02007f0c, 1, 0x318, 0x1b0);
    Func_02007f8e(1, 0, 0);
    Call3(Func_02007f24, 8, 0x328, 0x198);
    Call3(Func_02007fa8, 8, 0x8000, 0);
    Call3(Func_02007f3e, 1, 0x328, 0x1b0);
    Call3(Func_02007fc2, 1, 0xc000, 0);
    Func_02007ed8(20);
    Func_020077e2(8, 3, 20);
    Func_020077a2(8, 20);
    Call3(Func_02007f70, 8, 0x300, 0x198);
    Func_02007efe(20);
    Func_02007fd0(8, 0, 0);
    Func_02007fda(0, 8, 0);
    Func_02007fe4(1, 8, 0);
    Func_020077f6(2, 8, 40);
    Func_020077e6(8, 30);
    Func_02007fce(0, 3);
    Func_02007fd6(1, 3);
    Func_02007848(2, 3, 20);
    Call3(Func_02007fce, 8, 0x2e8, 0x198);
    Func_02007f5c(50);
    Func_02008024(11, 2);
    Func_02007824(11, 20);
    Func_02007fc4(11, base5_200d17c);
    Func_0200803c(1, 1);
    Func_02007f82(20);
    Func_02008054(0, 11, 0);
    Func_0200805e(1, 11, 0);
    Func_02007870(2, 11, 20);
    Func_02007860(1, 20);
    Func_02008070(12, 2);
    Func_02007870(12, 30);
    Func_02008010(12, base5_200d17c);
    Call3(Func_020080ca, 1, 0x103, 0);
    Func_02007fd0(60);
    Func_02007890(1, 30);
    Func_020078b2(2, 0, 30);
    Func_020078bc(0, 2, 30);
    Func_020078f6(2, 3, 20);
    Func_020078b6(2, 20);
    Func_020078d8(1, 2, 30);
    Func_02007912(0, 3, 30);
    Func_020078d2(2, 20);
    Call3(Func_02008126, 1, 0x100, 0);
    Func_0200802c(60);
    Func_020078ec(1, 30);
    Call3(Func_0200813e, 2, 0x101, 0);
    Func_02008044(60);
    Func_02007936(0, 1, 20);
    Func_020080ee(0, 3);
    Func_02007960(1, 3, 20);
    Func_02007984(2, 0);
    Func_0200822c();
    Func_0200813e(0, 2, 0);
    Func_02007950(1, 2, 10);
    Func_02008140(0, 1);
    Func_02008150(1, 1);
    Func_02008096(20);
    Call2(Func_020081a8, 1, 0x102);
    Func_020080a6(60);
    Func_02007980(0, 1, 10);
    Value2(Func_02008198, 1, 0);
    if (Value2(Func_020080f8, 0, 0) == 0) {
        Func_020080ca(20);
        Func_0200819c(0, 2, 0);
        Func_020079ae(1, 2, 20);
        Func_02007a2a();
        Func_020081a2(0, 1);
        Func_020081b2(1, 1);
        ((void (*)())Func_020080f8)(20);
        Func_020079b8(1, 20);
    } else {
        bump_step(1);
        Call3(Func_02008238, 1, 0x102, 0);
        Func_0200813e(60);
        Func_020079fe(1, 20);
        Func_02008218(0, 2, 0);
        Func_02007a2a(1, 2, 20);
        Func_02007aa6();
        Func_0200821e(0, 1);
        Func_0200822e(1, 1);
        Func_02008174(20);
    }
    Call3(Func_02008280, 2, 0x102, 0);
    Func_02008186(60);
    Func_02007a90(2, 4, 20);
    Call1(Func_0200826e, 0x12f2);
    Func_02007a56(2, 20);
    Func_0200823e(0, 3);
    Func_02007ab0(1, 3, 40);
    Call3(Func_02008236, 8, 0x328, 0x198);
    Call3(Func_020082ba, 8, 0x8000, 0);
    Func_020081d0(30);
    Func_020082a2(0, 8, 0);
    Func_020082ac(1, 8, 0);
    Func_02007abe(2, 8, 20);
    Func_020082b6(8, 1);
    Func_020081fc(20);
    Func_02007abc(8, 20);
    Call3(Func_02008256, 9, 0xcccc, 0x6666);
    Call3(Func_02008260, 13, 0xcccc, 0x6666);
    Call3(Func_0200826a, 14, 0xcccc, 0x6666);
    Call3(Func_020082c0, 9, 0x2e80000, 0x1980000);
    Call3(Func_020082b6, 9, 0x300, 0x198);
    Func_02007b18(9, 10, 30);
    Call3(Func_020082e6, 13, 0x2e80000, 0x1980000);
    Call3(Func_020082dc, 13, 0x300, 0x198);
    Call3(Func_02008302, 14, 0x2e80000, 0x1980000);
    Call3(Func_020082f0, 14, 0x310, 0x190);
    Call3(Func_02008306, 13, 0x308, 0x1a8);
    Func_0200831c(14);
    Func_02008366(13, 10, 0);
    Func_02007b78(14, 10, 20);
    Func_0200837a(0, 9, 0);
    Func_02008384(1, 9, 0);
    Func_02007b96(2, 9, 20);
    Func_02007bd0(9, 4, 20);
    Func_02007b90(9, 20);
    Func_02007b98(11, 20);
    Func_02007ba0(12, 30);
    Func_02007bda(9, 13, 20);
    Func_020083ba(13, 1);
    Func_02008300(20);
    Func_02007bc0(13, 20);
    Func_02007c12(9, 3, 30);
    Func_02007c04(9, 14, 20);
    Func_02007c26(9, 3, 30);
    Func_02007c00(9, 10, 20);
    Func_02007bf0(9, 20);
    Func_020083d8(13, 3);
    Func_02007c4a(14, 3, 20);
    Func_02007c3c(13, 14, 20);
    Func_020083f4(13, 3);
    Func_02007c66(14, 3, 20);
    Call3(Func_020083e4, 14, 0x318, 0x188);
    Call3(Func_020083fa, 13, 0x310, 0x190);
    Func_02008454(13, 12, 0);
    Func_0200841a(14);
    Func_02007c6c(14, 11, 20);
    Func_02008464(13, 1);
    Func_020083aa(20);
    Func_02007c6a(13, 20);
    Func_02007cbc(14, 4, 20);
    Func_02007c7c(14, 30);
    Func_02007cb6(13, 0, 20);
    Func_02007c8e(13, 20);
    Func_02008476(0, 3);
    Func_0200847e(1, 3);
    Func_02007cf0(2, 3, 50);
    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)((work + 0x1c8)) = 30;
    *(volatile s32 *)((work + 0x1c0)) = 0x201;
    Func_02008574();
    Func_02008580();
    Func_02008416(60);
    Func_0200842a();
}
