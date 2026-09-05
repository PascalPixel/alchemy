#include "types.h"

#define FieldScene_BuildActorPresentationGroup Func_02001298

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02003128();
void Func_02003e9a();
void Func_02003ec6();
void Func_02003ed6();
void Func_02003f22();
s32 Func_02003f28();
void Func_02003f2a();
void Func_02003f32();
s32 Func_02003f34();
void Func_02003f3a();
s32 Func_02003f3e();
void Func_02003f42();
void Func_02003f4a();
void Func_02003f52();
void Func_02003f5a();
void Func_02003f60();
void Func_02003f62();
void Func_02003f6a();
void Func_02003f6e();
void Func_02003f72();
void Func_02003f8a();
void Func_02003f98();
void Func_02003f9c();
void Func_02003fa0();
void Func_02003fa6();
void Func_02003fb4();
void Func_02003fe4();
void Func_02003ff2();
void Func_02003ff8();
void Func_02004000();
void Func_02004002();
void Func_0200400e();
void Func_02004010();
void Func_0200401c();
void Func_0200401e();
void Func_02004020();
void Func_0200402c();
void Func_02004044();
void Func_02004046();
void Func_0200405c();
void Func_02004086();
void Func_02004088();
void Func_0200408a();
void Func_02004098();
void Func_0200409a();
void Func_020040a8();
void Func_020040d8();
s32 Func_020040e4();
void Func_020040e6();
void Func_020040f6();
void Func_020040fc();
void Func_02004106();
void Func_02004108();
void Func_02004114();
void Func_0200411e();
void Func_0200412a();
void Func_0200413a();
void Func_02004144();
void Func_02004148();
void Func_0200414e();
void Func_02004150();
void Func_02004158();
void Func_0200415e();
void Func_02004162();
void Func_02004164();
void Func_02004166();
void Func_02004170();
void Func_0200417c();
void Func_0200417e();
void Func_02004180();
void Func_0200418c();
void Func_02004194();
void Func_0200419c();
void Func_020041b4();
void Func_020041c0();
void Func_020041c2();
void Func_020041ca();
s32 Func_020041d6();
void Func_020041d8();
void Func_020041e2();
void Func_020041e4();
void Func_020041ea();
void Func_020041f0();
void Func_02004202();
void Func_020042d4();
void Func_020042e6();
void Func_020042f2();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_BuildActorPresentationGroup(void)
{
    u32 i;
    s32 p9;
    s32 record;
    s32 base10_1c0;

    Func_02003e9a();
    Call4(Func_02003f9c, -1, -1, -1, 0);
    Func_02003ff2(247);
    Func_02003f22(8, 2);
    Func_02003f2a(9, 2);
    Func_02003f32(10, 2);
    Func_02003f3a(11, 2);
    Func_02003f42(12, 2);
    Func_02003f4a(13, 2);
    Func_02003f52(14, 0);
    Func_02003f5a(15, 0);
    Func_02003f62(16, 0);
    Func_02003f6a(17, 0);
    Func_02003f72(18, 0);
    record = Func_02003f28(21);
    Func_02003ec6(record, 0);
    record = Func_02003f34(19);
    *(s32 *)(record + 24) = -0x10000;
    record = Value1(Func_02003f3e, 20);
    *(s32 *)(record + 24) = -0x10000;
    Func_02003ec6(1);
    Func_02003f98(0, 0, 0);
    Func_02003ed6(1);
    {
        u8 *work = *(u8 **)0x03001ebc;

        base10_1c0 = 224;
        base10_1c0 <<= 1;
        *(s32 *)(work + base10_1c0) = 0x200;
        p9 = 456;
        *(s32 *)(work + p9) = 32;
    }
    Func_02004086();
    Func_0200409a();
    Func_02003f60(40);
    Func_02003ff8(16, 1);
    Func_02003f6e(20);
    Call3(Func_02003fa0, 16, 0xcccc, 0x6666);
    Call3(Func_02003fe4, 16, 164, 0x388);
    Func_02003f8a(20);
    Func_02004002(16, 9);
    Func_02003f98(40);
    Func_02004010(16, 10);
    Func_02003fa6(60);
    Func_0200401e(16, 1);
    Func_02003fb4(20);
    Call3(Func_02004020, 16, 164, 0x398);
    Call3(Func_0200402c, 16, 185, 0x398);
    Call3(Func_020040a8, 16, 0xc000, 20);
    Call3(Func_02004044, 16, 185, 0x394);
    Func_0200405c(16, 11);
    Func_02003ff2(40);
    Func_0200408a(16, 1);
    Func_02004000(60);
    Func_02004098(16, 3);
    Func_0200400e(40);
    Call2(Func_02004046, 16, 0x200af88);
    Func_0200401c(80);
    Call2(Func_02004106, 16, 0x102);
    Func_0200402c(60);
    Call3(Func_02004108, 14, 0xd000, 0);
    Call3(Func_02004114, 15, 0x5000, 0);
    Func_0200411e(17, 0, 0);
    Call3(Func_0200412a, 18, 0x8000, 20);
    Call2(Func_02004144, 14, 0x102);
    Call2(Func_0200414e, 15, 0x102);
    Call2(Func_02004158, 17, 0x102);
    Call2(Func_02004162, 18, 0x102);
    Func_02004088(60);
    Call3(Func_02004164, 14, 0x3000, 0);
    Call3(Func_02004170, 15, 0x3000, 0);
    Call3(Func_02004180, 17, 0x3000, 0);
    Func_02003128(18, 0x3000);
    Func_020040f6(16);
    record = Func_020040e4(16);
    {
        s32 shown = 0xd000;

        *(u16 *)(record + 6) = shown;
    }
    *(s32 *)(record + 24) = 0x10000;
    *(s32 *)(record + 28) = 0x10000;
    Func_020040d8(20);
    Func_02004150(16, 0);
    Func_020040e6(40);
    Func_0200415e(19, 5);
    Func_02004166(20, 5);
    Func_020040fc(60);
    Value3(Func_020041d6, 16, 0x3000, 20);
    Func_0200417e(16, 8);
    Func_02004114(20);
    Func_0200418c(14, 4);
    Func_02004194(15, 4);
    Func_0200419c(17, 4);
    Func_020041b4(18, 4);
    Func_0200413a(40);
    Func_020041c2(16, 4);
    Func_02004148(10);
    Call3(Func_0200417c, 16, 0x20000, 0x10000);
    Call3(Func_020041c0, 16, 162, 0x394);
    Func_020041ca(16, 162, 0x37a);
    Func_020041e2(19, 1);
    Func_020041ea(20, 1);
    Call3(Func_020041e4, 16, 184, 0x35f);
    Call3(Func_020041f0, 16, 184, 0x31c);
    Func_02004202(16, 0, 0);
    {
        u8 *work = *(u8 **)0x03001ebc;

        *(s32 *)(work + base10_1c0) = 0x201;
        *(s32 *)(work + p9) = 16;
    }
    Func_020042e6();
    Func_020042f2();
    *(s32 *)(*(u8 **)0x03001ebc + base10_1c0) = 0x100;
    Func_020042d4(69);
    Func_020041d8();
}
