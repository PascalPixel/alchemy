#include "types.h"

#define FieldScene_RunMiddleSequence Func_02001688

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_020034f2();
void Func_02004280();
void Func_020042ae();
void Func_020042be();
void Func_02004308();
s32 Func_02004310();
void Func_02004318();
s32 Func_0200431c();
void Func_02004320();
s32 Func_02004326();
void Func_02004328();
void Func_02004330();
void Func_02004338();
void Func_02004340();
void Func_02004342();
void Func_02004350();
void Func_02004352();
void Func_0200435a();
void Func_0200436c();
void Func_0200437a();
void Func_02004380();
void Func_02004382();
void Func_02004388();
void Func_02004396();
void Func_020043c6();
void Func_020043d4();
void Func_020043d8();
void Func_020043da();
void Func_020043e2();
void Func_020043e4();
void Func_020043f0();
void Func_020043f2();
void Func_020043fe();
void Func_02004400();
void Func_02004402();
void Func_0200440e();
void Func_02004426();
void Func_02004428();
void Func_0200443e();
void Func_0200444a();
void Func_0200445a();
void Func_02004460();
void Func_02004468();
void Func_0200446c();
void Func_0200447a();
void Func_0200447c();
void Func_0200448a();
s32 Func_020044b0();
void Func_020044d4();
void Func_020044da();
void Func_020044e8();
void Func_020044ea();
void Func_020044f4();
void Func_020044f6();
void Func_02004500();
void Func_02004502();
void Func_0200450a();
s32 Func_02004512();
void Func_0200451c();
void Func_02004520();
void Func_02004526();
void Func_02004530();
void Func_02004536();
void Func_0200453a();
void Func_0200453c();
void Func_0200453e();
void Func_02004548();
void Func_0200455c();
void Func_0200456a();
void Func_0200456e();
void Func_0200457c();
void Func_0200458a();
void Func_02004594();
void Func_02004598();
void Func_020045ae();
void Func_020045be();
void Func_020045d6();
void Func_020045d8();
void Func_020045e6();
void Func_020045f4();
void Func_02004602();
void Func_02004630();
void Func_02004658();
void Func_0200466e();
void Func_0200469c();
void Func_020046d2();
void Func_020046ea();
void Func_020046f6();

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

void FieldScene_RunMiddleSequence(void)
{
    u32 i;
    u8 *rec8;
    s32 record;
    u8 *work;

    Func_02004280();
    Call4(Func_02004382, -1, -1, -1, 0);
    Func_020043d8(247);
    Func_02004308(8, 2);
    Call2((void (*)())Func_02004310, 9, 2);
    Func_02004318(10, 2);
    Func_02004320(11, 2);
    Func_02004328(12, 2);
    Func_02004330(13, 2);
    Func_02004338(14, 0);
    Func_02004340(15, 0);
    Func_02004342(16, 0, 0);
    Func_02004352(17, 0);
    Func_0200435a(18, 0);
    record = Func_02004310(21);
    Func_020042ae(record, 0);
    record = Func_0200431c(19);
    *(volatile s32 *)(record + 24) = -0x10000;
    record = Value1(Func_02004326, 20);
    *(volatile s32 *)(record + 24) = -0x10000;
    Func_020042ae(1);
    Func_02004380(0, 0, 0);
    Func_020042be(1);
    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)((work + 0x1c0)) = 0x200;
    *(volatile s32 *)((work + 0x1c8)) = 32;
    Func_02004468();
    Func_0200447c();
    Func_02004342(40);
    Func_020043da(17, 1);
    Func_02004350(20);
    Call3(Func_02004382, 17, 0xcccc, 0x6666);
    Call3(Func_020043c6, 17, 164, 0x388);
    Func_0200436c(20);
    Func_020043e4(17, 9);
    Func_0200437a(40);
    Func_020043f2(17, 10);
    Func_02004388(60);
    Func_02004400(17, 1);
    Func_02004396(20);
    Call3(Func_02004402, 17, 164, 0x398);
    Call3(Func_0200440e, 17, 185, 0x398);
    Call3(Func_0200448a, 17, 0xc000, 20);
    Call3(Func_02004426, 17, 185, 0x394);
    Func_0200443e(17, 11);
    Func_020043d4(40);
    Func_0200446c(17, 1);
    Func_020043e2(60);
    Func_0200447a(17, 3);
    Func_020043f0(40);
    Call2(Func_02004428, 17, 0x200af88);
    Func_020043fe(80);
    Call2(Func_020044e8, 17, 0x102);
    Func_0200440e(60);
    Call3(Func_020044ea, 14, 0xd000, 0);
    Call3(Func_020044f6, 15, 0x5000, 0);
    Call3(Func_02004502, 18, 0x8000, 20);
    Call2(Func_0200451c, 14, 0x102);
    Call2(Func_02004526, 15, 0x102);
    Call2(Func_02004530, 17, 0x102);
    Call2(Func_0200453a, 18, 0x102);
    Func_02004460(60);
    Call3(Func_0200453c, 14, 0x3000, 0);
    Call3(Func_02004548, 15, 0x3000, 0);
    Value2(Func_020034f2, 18, 0x3000);
    Call2(Func_0200456a, 17, 0x101);
    rec8 = Value1(Func_020044b0, 21);
    rec8[85] = 0;
    for (i = 0; i < 20; i++) {
        *(volatile s32 *)(rec8 + 12) += 0x9999;
        Func_0200444a(4);
        *(volatile s32 *)(rec8 + 12) += -0x4ccc;
        Func_0200445a(4);
    }
    Func_02004536(19, 6);
    Func_0200453e(20, 6);
    Func_020044d4(60);
    Call2(Func_020045be, 17, 0x100);
    Func_0200451c(17);
    Func_0200455c(17, 1);
    record = Func_02004512(17);
    {
        s32 shown = 0xd000;
    
        *(volatile u16 *)(record + 6) = shown;
    }
    rec8[85] = 3;
    *(volatile s32 *)(rec8 + 24) = 0x10000;
    *(volatile s32 *)(rec8 + 28) = 0x10000;
    Func_0200450a(10);
    Func_02004658(107);
    Func_020044da(0x10000, 0x10000, 0x10000);
    Func_02004520(10);
    Call1(Func_0200466e, 0x121);
    Call3(Func_020044f4, -1, -1, 0xe666);
    Func_02004500();
    Call3(Func_0200456a, 17, 0x19999, 0xcccc);
    Call3(Func_020045ae, 17, 208, 0x3a0);
    Func_0200469c(92);
    Call3(Func_02004630, 17, 0x3000, 20);
    Func_020045d8(17, 9);
    Func_0200456e(20);
    Func_020045e6(17, 10);
    Func_0200457c(40);
    Func_020045f4(17, 9);
    Func_0200458a(20);
    Func_02004602(17, 10);
    Func_02004598(80);
    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)((work + 0x1c0)) = 0x201;
    *(volatile s32 *)((work + 0x1c8)) = 16;
    Func_020046ea();
    Func_020046f6();
    Call1(Func_02004594, 0x8a4);
    Func_020046d2(69);
    Func_020045d6();
}
