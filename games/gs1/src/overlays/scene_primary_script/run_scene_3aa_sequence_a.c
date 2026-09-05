#include "types.h"

#define FieldScene_RunScene3aaSequenceA Func_02001494

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02002a2e();
void Func_02002d34();
void Func_02002ee4();
void Func_02002f00();
void Func_02002f02();
void Func_02002f1c();
void Func_02002fbc();
void Func_02002fc6();
void Func_02002fd0();
void Func_02002fda();
void Func_02002fe4();
s32 Func_02002ff0();
void Func_02002ffe();
s32 Func_02003004();
void Func_02003012();
void Func_02003016();
s32 Func_02003018();
void Func_0200303e();
void Func_0200304a();
void Func_02003052();
void Func_02003054();
void Func_02003062();
void Func_02003064();
void Func_02003066();
void Func_02003070();
void Func_02003072();
void Func_02003086();
void Func_020030ae();
void Func_020030b6();
void Func_020030be();
void Func_020030e2();
void Func_02003106();
void Func_02003124();
void Func_02003128();
void Func_02003148();
void Func_0200314a();
void Func_0200314e();
void Func_0200315c();
void Func_02003160();
void Func_0200316a();
void Func_02003170();
s32 Func_02003176();
void Func_02003186();
void Func_02003190();
void Func_0200319a();
void Func_020031a4();
void Func_020031ac();
void Func_020031ae();
void Func_020031b2();
void Func_020031b6();
void Func_020031be();
void Func_020031c8();
void Func_020031ce();
void Func_020031d8();
void Func_020031dc();
void Func_020031e0();
s32 Func_020031e4();
void Func_020031f6();
void Func_020031fc();
void Func_0200322e();
void Func_0200323a();
void Func_0200326e();
void Func_02003270();
void Func_02003280();
void Func_0200328c();
void Func_02003298();
void Func_0200329e();
void Func_020032b8();
void Func_020032ba();
void Func_020032be();
void Func_020032c0();
void Func_020032c2();
void Func_020032ca();
void Func_020032ce();
void Func_020032e0();
void Func_020032e6();
void Func_020032ea();
void Func_020032f2();
void Func_020032f4();
void Func_020032fc();
void Func_02003310();
s32 Func_02003312();
void Func_02003324();
void Func_0200333a();
void Func_02003340();
void Func_02003342();
void Func_02003356();
void Func_0200335a();
void Func_02003368();
s32 Func_02003382();
void Func_02003392();
void Func_020033c2();
void Func_020033d2();
void Func_020033f0();
void Func_020033f2();
void Func_02003402();
void Func_02003410();
void Func_02003418();
void Func_0200341a();
void Func_0200342c();
void Func_02003446();
void Func_02003448();
void Func_0200344a();
void Func_0200345e();
void Func_02003464();
void Func_02003466();
void Func_0200346c();
void Func_02003476();
void Func_02003478();
void Func_0200347c();
void Func_02003482();
void Func_02003488();
void Func_0200348a();
void Func_0200348e();
s32 Func_02003492();
void Func_0200349a();
void Func_020034a0();
s32 Func_020034a2();
void Func_020034aa();
void Func_020034b2();
void Func_020034ba();
void Func_020034ca();

extern u8 Data_03001ebc[];
extern u8 Data_02009b94[];

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

void FieldScene_RunScene3aaSequenceA(void)
{
    u32 i;
    s32 record;

    Func_02002f1c();
    Call4(Func_02002ffe, -1, -1, -1, 0);
    Func_02002ee4(1);
    Call4(Func_02003016, 0x3600000, -1, 0x2180000, 0);
    Func_02002f02();
    Func_02002f00(1);
    Call3(Func_02002fbc, 0, 0x3600000, 0x2760000);
    *(s32 *)((*(u8 **)Data_03001ebc + 0x1c0)) = 0x100;
    *(s32 *)((*(u8 **)Data_03001ebc + 0x1c8)) = 40;
    Func_02003072();
    Call2(Func_0200304a, 0x6666, 0xccc);
    Call4(Func_02003064, 0x3600000, -1, 0x1d80000, 1);
    Call3(Func_02002fc6, 0, 0xcccc, 0x6666);
    Call3(Func_02002fd0, 1, 0xcccc, 0x6666);
    Call3(Func_02002fda, 2, 0xcccc, 0x6666);
    Call3(Func_02002fe4, 3, 0xcccc, 0x6666);
    Call3(Func_02003012, 0, 0x360, 0x1f2);
    record = Value1(Func_02002ff0, 0);
    if (record != 0) {
        Func_0200303e(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Func_02003004, 0);
    if (record != 0) {
        Func_02003052(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Value1(Func_02003018, 0);
    if (record != 0) {
        Func_02003066(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Call3(Func_02003054, 0, 0x358, 0x1e6);
    Call3(Func_02003062, 1, 0x350, 0x1f6);
    Call3(Func_02003070, 2, 0x368, 0x1e6);
    Call3(Func_02003086, 3, 0x370, 0x1f6);
    Func_020030ae(0, 1);
    Func_020030b6(1, 1);
    Func_020030be(2, 1);
    Func_02003054(10);
    Func_02002a2e(10);
    Func_02003124(9, 0x100, 20);
    Call3(Func_02003128, 9, 0x5000, 20);
    Call1(Func_02003106, 0x2588);
    Call3(Func_02003128, 0x2009, 0, 10);
    Func_0200314a(8, 0x100, 20);
    Call3(Func_0200314e, 8, 0x3000, 20);
    Func_02003148(8, 0, 20);
    Call3(Func_0200316a, 8, 0x107, 60);
    Func_0200315c(8, 0, 10);
    Call2(Func_02003186, 0, 0x102);
    Call2(Func_02003190, 1, 0x102);
    Call2(Func_0200319a, 2, 0x102);
    Call2(Func_020031a4, 3, 0x102);
    Func_020030e2(60);
    Call3(Func_020031ae, 9, 0x102, 60);
    Call3(Func_020031b2, 9, 0x7000, 10);
    Call3(Func_020031ac, 0x2009, 0, 10);
    Call3(Func_020031c8, 8, 0x1000, 10);
    Call3(Func_020031dc, 8, 0x108, 20);
    Func_020031ce(8, 0, 40);
    Func_020031b6(8, 2);
    Func_020031e0(8, 0, 20);
    Call3(Func_020031fc, 8, 0x3000, 10);
    Value2(Func_020031e4, 8, 0);
    i = 1;
    if (Value2(Func_02003176, 0, 0) == 0) {
        Func_02003160(10);
        Func_020031d8(8, 3);
    } else {
        Func_02003170(10);
        bump_step(1);
        Func_020031f6(8, 4);
        i = 0;
    }
    Func_0200323a(8, 0, 10);
    if (i != 0) {
        bump_step(1);
    }
    Func_0200322e(9, 2);
    Call2(Func_02003280, 9, 0x102);
    Func_020031be(80);
    Call3(Func_02003270, 0x2009, 0, 10);
    Call3(Func_0200328c, 8, 0x1000, 10);
    Call3(Func_0200329e, 8, 0x107, 40);
    Func_0200326e(2, 3);
    Call3(Func_02003298, 0x2002, 0, 20);
    Call3(Func_020032ba, 8, 0x105, 60);
    Call3(Func_020032be, 8, 0x3000, 60);
    Func_020032b8(8, 0, 10);
    Func_02003298(1, 2);
    Func_020032ca(1, 0, 10);
    Call3(Func_020032e6, 9, 0x5000, 0);
    Call3(Func_020032f2, 2, 0x6000, 10);
    Func_020032c2(2, 2);
    Call3(Func_020032f4, 0x6002, 0, 10);
    Call3(Func_02003310, 2, 0xc000, 10);
    Func_020032e0(2, 2);
    Call3(Func_02003312, 0x2002, 0, 20);
    Func_020032ea(8, 3);
    Func_02003324(8, 0, 20);
    Call3(Func_02003340, 2, 0x8000, 20);
    Call3(Func_0200333a, 0x6002, 0, 10);
    Call3(Func_02003356, 3, 0xa000, 10);
    Call3(Func_02003368, 3, 0x105, 40);
    Func_0200335a(3, 0, 10);
    Func_02003342(2, 2);
    Func_020032c0(80);
    Func_02003340(2, 3);
    Func_020032ce(20);
    Call3(Func_02003392, 1, 0xe000, 10);
    Func_0200335a(1, 4);
    Value2(Func_02003382, 1, 0);
    if (Value2(Func_02003312, 0, 0) == 0) {
        Func_020032fc(20);
        bump_step(1);
    } else {
        Func_02003340(20);
        Func_020033f2(1, 0, 10);
    }
    Func_020033c2(2, 3);
    Func_020033d2(3, 3);
    Func_02002d34(20);
    Func_020033f0(9, 2);
    Call3(Func_0200341a, 0x2009, 0, 10);
    Func_02003402(3, 2);
    Func_0200342c(3, 0, 10);
    Call3(Func_02003448, 9, 0x3000, 10);
    Func_02003410(9, 3);
    Call3(Func_0200344a, 0x2009, 0, 10);
    Call3(Func_0200346c, 2, 0x101, 60);
    Call3(Func_0200345e, 0x2002, 0, 10);
    Func_02003446(9, 1);
    Call3(Func_02003482, 9, 0x5000, 10);
    Call3(Func_0200347c, 0x2009, 0, 10);
    Func_02003464(1, 2);
    Func_0200348e(1, 0, 10);
    Func_02003466(9, 4);
    Call3(Func_020034a0, 0x2009, 0, 10);
    Func_02003488(3, 1);
    Func_020034b2(3, 0, 10);
    Func_0200349a(8, 1);
    Func_02003418(20);
    Func_020034ca(8, 0, 10);
    Func_0200349a(0, 3);
    Call2((void (*)())Func_020034a2, 1, 3);
    Func_020034aa(2, 3);
    Func_020034ba(3, 3);
    Func_02003448(20);
    Func_0200348a(1, (s32)Data_02009b94);
    Value2(Func_02003492, 2, (s32)Data_02009b94);
    Value2(Func_020034a2, 3, (s32)Data_02009b94);
    *(s32 *)((*(u8 **)Data_03001ebc + 0x1c8)) = 16;
    *(s32 *)((*(u8 **)Data_03001ebc + 0x1c0)) = 0x209;
    Call1(Func_02003478, 0x12f);
    Call1(Func_02003476, 0x914);
    Func_0200349a();
}
