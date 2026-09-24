#include "TYPES.H"


void Func_020024ae();
s32 Func_02003472();
void Func_02003488();
void Func_0200349e();
void Func_020034aa();
s32 Func_020034c0();
s32 Func_020034e4();
void Func_020034fe();
s32 Func_02003500();
void Func_02003696();
void Func_020036b2();
void Func_020036da();
void Func_020036f4();
void Func_0200372e();
void Func_0200377a();
void Func_020037c4();
void Func_020037d6();
void Func_020037fc();
void Func_02003800();
void Func_02003806();
void Func_02003826();
s32 Func_02003850();
void Func_02003858();
void Func_02003862();
void Func_02003864();
void Func_0200386c();
void Func_02003870();
void Func_02003884();
void Func_0200388a();
void Func_020038a2();
void Func_020038be();
void Func_020038c2();
void Func_020038c6();
void Func_020038d4();
void Func_020038e2();
void Func_020038e6();
void Func_020038e8();
void Func_020038ec();
void Func_020038f2();
void Func_020038f8();
void Func_02003904();
void Func_02003914();
void Func_02003920();
void Func_02003924();
void Func_02003948();
void Func_0200394c();
void Func_0200395c();
void Func_02003962();
void Func_0200396c();
void Func_02003988();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* NONMATCHING: 496 of 496 bytes, 11 halfword edits (2026-09-24). Script call run;
 * 0x8017 and 0x2018 are shared constants whose registers (r6/r8) and pool
 * order still differ from the reference. */
void HaidiaIe_Func02001084(void)
{
    u8 *rec7;

    rec7 = Value0(Func_02003850);
    Func_0200372e();
    Call4(Func_02003858, -1, -1, -1, 0);
    Func_02003696(1);
    Call4(Func_02003870, 0x400000, 0x900000, 0x15e0000, 0);
    Func_020036f4();
    Func_020036b2(1);
    Func_0200377a(1, 0);
    Func_020038e6();
    Func_02003924(17);
    Func_020038e8();
    Call3(Func_020037fc, 23, 0x690000, 0x10b0000);
    Func_020036da(1);
    Call3(Func_020037c4, 0, 0x13333, 0x9999);
    Call3(Func_02003806, 0, 93, 0x157);
    Call1(Func_0200386c, 0xed6);
    Func_02003884(23, 0);
    Func_02003962(61);
    rec7[85] = 0;
    Call2(Func_020038d4, 0x30000, 0x6000);
    Call4(Func_020038ec, 0x6d0000, 0xb00000, 0x1190000, 1);
    Func_020038f8();
    Func_020037d6(40);
    Call3(Func_02003864, 24, 0x870000, 0xb10000);
    Call3(Func_02003826, 24, 0xcccc, 0x6666);
    Call3(Func_02003862, 24, 126, 0x102);
    Func_02003800(40);
    Call3(Func_02003904, 23, 0xd000, 0);
    Func_0200388a(24);
    Func_020038a2(24, 1);
    Value3(Func_02003472, 24, 0x7000, 10);
    Func_020038be(23, 3);
    Func_020038c6(24, 4);
    Call2(Func_02003920, 0x2018, 0);
    Func_020038f2(23, 2);
    Func_02003488(0x8017, 30);
    Func_020034aa(24, 0xb000, 20);
    Func_0200349e(0x2018, 10);
    Value3(Func_020034c0, 23, 0xb000, 40);
    Func_0200395c(0x8017, 0);
    Func_02003914(24, 4);
    Func_0200396c(0x2018, 0);
    Value3(Func_020034e4, 23, 0xf000, 10);
    Func_02003948(23, 2);
    Func_02003988(0x8017, 0);
    Value3(Func_02003500, 24, 0x6000, 20);
    Func_0200394c(24, 3);
    Func_020038c2(20);
    Func_020034fe(0x2018, 20);
    Func_020024ae();
    Func_020038e2();
}
