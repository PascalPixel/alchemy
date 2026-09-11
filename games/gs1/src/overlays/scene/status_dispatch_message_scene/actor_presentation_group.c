#include "types.h"

/* resource_3b9:0x02001298, 884 bytes: the middle presentation script for
 * actor 16, a sibling of FieldScene_RunMiddleSequence (0x02001688) with the
 * same opening, scene-phase writes and closing, without the stepping loop. */

#define FieldScene_BuildActorPresentationGroup Func_02001298

extern u8 Data_03001ebc[];

void Func_02003e9a();
void Func_02003f9c();
void Func_02003ff2_a();
void Func_02003ff2_b();
void Func_02003f22();
void Func_02003f2a();
void Func_02003f32();
void Func_02003f3a();
void Func_02003f42();
void Func_02003f4a();
void Func_02003f52();
void Func_02003f5a();
void Func_02003f62();
void Func_02003f6a();
void Func_02003f72();
s32 Func_02003f28();
void Func_02003ec6_a();
void Func_02003ec6_b();
s32 Func_02003f34();
s32 Func_02003f3e();
void Func_02003f98_a();
void Func_02003f98_b();
void Func_02003ed6();
void Func_02004086();
void Func_0200409a();
void Func_02003f60();
void Func_02003ff8();
void Func_02003f6e();
void Func_02003fa0();
void Func_02003fe4();
void Func_02003f8a();
void Func_02004002();
void Func_02004010();
void Func_02003fa6();
void Func_0200401e();
void Func_02003fb4();
void Func_02004020();
void Func_0200402c_a();
void Func_0200402c_b();
void Func_020040a8();
void Func_02004044();
void Func_0200405c();
void Func_0200408a();
void Func_02004000();
void Func_02004098();
void Func_0200400e();
void Func_02004046();
void Func_0200401c();
void Func_02004106();
void Func_02004108();
void Func_02004114_a();
void Func_02004114_b();
void Func_0200411e();
void Func_0200412a();
void Func_02004144();
void Func_0200414e();
void Func_02004158();
void Func_02004162();
void Func_02004088();
void Func_02004164();
void Func_02004170();
void Func_02004180();
s32 Func_02003128();
void Func_020040f6();
s32 Func_020040e4();
void Func_020040d8();
void Func_02004150();
void Func_020040e6();
void Func_0200415e();
void Func_02004166();
void Func_020040fc();
void Func_020041d6();
void Func_0200417e();
void Func_0200418c();
void Func_02004194();
void Func_0200419c();
void Func_020041b4();
void Func_0200413a();
void Func_020041c2();
void Func_02004148();
void Func_0200417c();
void Func_020041c0();
void Func_020041ca();
void Func_020041e2();
void Func_020041ea();
void Func_020041e4();
void Func_020041f0();
void Func_02004202();
void Func_020042e6();
void Func_020042f2();
void Func_020042d4();
void Func_020041d8();

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
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}
static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

void FieldScene_BuildActorPresentationGroup(void)
{
    s32 rec2;
    u8 *work;
    s32 shift;
    s32 phase;
    s32 field;

    Func_02003e9a();
    Call4(Func_02003f9c, -1, -1, -1, 0);
    Func_02003ff2_a(247);
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
    rec2 = Func_02003f28(21);
    Func_02003ec6_a(rec2, 0);
    rec2 = Func_02003f34(19);
    *(volatile s32 *)(rec2 + 24) = -0x10000;
    rec2 = Value1(Func_02003f3e, 20);
    *(volatile s32 *)(rec2 + 24) = -0x10000;
    Func_02003ec6_b(1);
    Func_02003f98_a(0, 0, 0);
    Func_02003ed6(1);
    work = *(u8 *volatile *)Data_03001ebc;
    phase = 0x1c0;
    *(volatile s32 *)(work + phase) = 0x200;
    field = 0x1c8;
    *(volatile s32 *)(work + field) = 32;
    Func_02004086();
    Func_0200409a();
    Func_02003f60(40);
    Func_02003ff8(16, 1);
    Func_02003f6e(20);
    Call3(Func_02003fa0, 16, 0xcccc, 0x6666);
    Func_02003fe4(16, 164, 0x388);
    Func_02003f8a(20);
    Func_02004002(16, 9);
    Func_02003f98_b(40);
    Func_02004010(16, 10);
    Func_02003fa6(60);
    Func_0200401e(16, 1);
    Func_02003fb4(20);
    Call3(Func_02004020, 16, 164, 0x398);
    Call3(Func_0200402c_a, 16, 185, 0x398);
    Call3(Func_020040a8, 16, 0xc000, 20);
    Call3(Func_02004044, 16, 185, 0x394);
    Func_0200405c(16, 11);
    Func_02003ff2_b(40);
    Func_0200408a(16, 1);
    Func_02004000(60);
    Func_02004098(16, 3);
    Func_0200400e(40);
    Func_02004046(16, 0x200af88);
    Func_0200401c(80);
    Call2(Func_02004106, 16, 0x102);
    Func_0200402c_b(60);
    Call3(Func_02004108, 14, 0xd000, 0);
    Call3(Func_02004114_a, 15, 0x5000, 0);
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
    shift = 0x3000;
    Value2(Func_02003128, 18, shift);
    Func_020040f6(16);
    rec2 = Func_020040e4(16);
    {
        s32 shown = 0xd000;
        *(volatile u16 *)(rec2 + 6) = shown;
    }
    *(volatile s32 *)(rec2 + 24) = 0x10000;
    *(volatile s32 *)(rec2 + 28) = 0x10000;
    Func_020040d8(20);
    Func_02004150(16, 0);
    Func_020040e6(40);
    Func_0200415e(19, 5);
    Func_02004166(20, 5);
    Func_020040fc(60);
    Func_020041d6(16, shift, 20);
    Func_0200417e(16, 8);
    Func_02004114_b(20);
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
    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)(work + phase) = 0x201;
    *(volatile s32 *)(work + field) = 16;
    Func_020042e6();
    Func_020042f2();
    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)(work + phase) = 0x100;
    Func_020042d4(69);
    Func_020041d8();
}
