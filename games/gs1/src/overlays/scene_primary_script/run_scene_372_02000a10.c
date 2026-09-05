#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000e5c[];
extern u8 Data_03001ebc[];
void Func_02005062();
void Func_02005110();
void Func_0200511a();
void Func_020051dc();
void Func_020051de();
void Func_020051e6();
void Func_020051e8();
void Func_020051f0();
void Func_0200523e();
void Func_02005248();
void Func_0200524c();
void Func_0200524e();
void Func_02005250();
void Func_0200525e();
void Func_02005264();
void Func_02005268();
void Func_02005270();
void Func_02005274();
void Func_0200527c();
void Func_02005284();
void Func_02005288();
void Func_020052a8();
void Func_020052aa();
void Func_020052b4();
void Func_020052c0();
void Func_020052d0();
void Func_020052ea();
void Func_020052ee();
void Func_020052fa();
void Func_02005304();
void Func_02005306();
void Func_0200531c();
void Func_02005320();
void Func_02005324();
void Func_02005328();
void Func_02005338();
void Func_0200533e();
void Func_02005344();
void Func_0200536a();
void Func_0200536c();
void Func_02005372();
void Func_0200537c();
void Func_0200538c();
void Func_02005392();
void Func_02005394();
void Func_02005396();
s32 Func_0200539c();
void Func_020053ae();
void Func_020053b6();
void Func_020053be();
void Func_020053c6();
void Func_020053c8();
void Func_020053ce();
void Func_020053d2();
void Func_020053e0();
void Func_020053e2();
void Func_020053e6();
void Func_020053f6();
void Func_0200540a();
void Func_0200540c();
void Func_02005410();
void Func_02005416();
s32 Func_0200541c();
void Func_02005420();
void Func_02005428();
void Func_0200542e();
void Func_02005438();
void Func_0200543c();
void Func_02005440();
void Func_0200544c();
void Func_02005454();
void Func_0200545a();
void Func_0200545e();
void Func_02005464();
void Func_02005474();
void Func_0200547c();
void Func_02005480();
void Func_02005484();

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

void FieldScene_RunScene372_02000a10(void)
{
    u32 i;
    s32 record;
    u8 *work;
    s32 base5_e5c;

    Func_02005110();
    Func_02005264();
    Func_02005270();
    Func_02005284();
    Func_02005062(60);
    Call2(Func_0200524e, 0x4000, 0x800);
    Call4(Func_02005268, 0x13c0000, 0xa00000, 0x3700000, 1);
    Call3(Func_020051de, 10, 0x1260000, 0x3640000);
    Func_020051e8(0, 0, 0);
    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0x100;
    *(volatile s32 *)(((s32)work + 0x1c8)) = 16;
    Func_020052fa();
    Func_02005306();
    Func_020052ea();
    Func_02005338(158);
    Call3(Func_0200511a, 0x200d78a, 50, 44);
    Call2(Func_020052aa, 22, 0x101);
    Call3(Func_020051dc, 9, 0xcccc, 0x6666);
    Call3(Func_020051e6, 0, 0xcccc, 0x6666);
    Call3(Func_020051f0, 10, 0xcccc, 0x6666);
    Call3(Func_0200524c, 9, 0x1560000, 0x37a0000);
    Call3(Func_02005248, 9, 0x156, 0x389);
    Func_0200537c();
    Call3(Func_02005250, 9, 0x128, 0x389);
    Call3(Func_02005274, 0, 0x1560000, 0x37a0000);
    Call3(Func_02005268, 0, 0x156, 0x37a);
    Call3(Func_0200527c, 0, 0x156, 0x389);
    Call3(Func_02005288, 0, 0x13e, 0x389);
    Func_020052a8(9, 1);
    Func_020052d0(9, 1);
    Call3(Func_02005324, 9, 0xc000, 60);
    base5_e5c = (s32)Data_00000e5c;
    Func_02005304(base5_e5c);
    Func_0200531c(9, 0);
    Call3(Func_020052c0, 10, 0x126, 0x346);
    Func_0200523e(40);
    Func_020052ee(10, 4);
    Func_0200533e(10, 0);
    Func_02005328(0, 9, 0);
    Func_0200525e(40);
    Call3(Func_02005372, 10, 0x4000, 0);
    Func_0200536c(10, 0, 20);
    Call3(Func_02005396, 9, 0x101, 20);
    Call3(Func_02005392, 9, 0xc000, 10);
    Func_0200538c(9, 0, 10);
    Func_02005344(10, 4);
    Func_02005394(10, 0);
    Call2(Func_020053ce, 9, 0x102);
    Func_020052b4(30);
    Func_020053c6(9, 0, 50);
    Call3(Func_020053d2, 9, 0xc000, 10);
    Call3(Func_02005320, 9, 0x18000, 0xc000);
    Call3(Func_0200536a, 9, 0x121, 0x373);
    Call3(Func_020053f6, 9, 0xe000, 0);
    Func_020053e6(9, 0);
    Func_020053be(10, 2);
    Func_020053f6(10, 0);
    Func_020053b6(9, 4);
    Func_02005410(9, 0, 10);
    Call3(Func_0200542e, 9, 0x2000, 10);
    Func_0200540c((base5_e5c + 8));
    Value2(Func_0200541c, 9, 0);
    Call3(Func_020053c8, 0, 0x12e, 0x389);
    Call3(Func_02005454, 0, 0xc000, 0);
    while (Value2(Func_0200539c, 0, 0) == 1) {
        Func_02005416(9, 1);
        Call1(Func_0200543c, 0xe65);
        Func_0200544c(9, 0);
    }
    Func_02005420(9, 3);
    Call1(Func_0200545e, 0xe66);
    Func_02005480(9, 0, 10);
    Func_02005438(0, 3);
    Call3(Func_020053e6, 10, 0x18000, 0xc000);
    Call3(Func_02005428, 10, 0x129, 0x2ee);
    Func_020053ae(10);
    Call3(Func_02005440, 9, 0x129, 0x2ee);
    Func_0200545a(9, 0, 0);
    Func_02005464(10, 0, 0);
    Func_02005474(10, 1);
    Func_0200547c(21, 2);
    Func_02005484(22, 5);
    Call1(Func_020053e2, 0x12f);
    Call1(Func_020053e0, 0x87b);
    Call1(Func_020053e6, 0x205);
    Func_0200540a();
}
