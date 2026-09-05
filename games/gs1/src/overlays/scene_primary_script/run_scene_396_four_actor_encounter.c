#include "types.h"

#define FieldScene_RunFourActorEncounter Func_02000918

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02009ec8[];
extern u8 Data_0200adc0[];
extern u8 Data_0200adc4[];
extern u8 Data_0200adc8[];
extern u8 Data_0200adcc[];
extern u8 Data_03001ebc[];
void Func_02001e14();
void Func_02001f34();
void Func_02001f46();
void Func_02001fc6();
void Func_0200205e();
void Func_020020b2();
void Func_02002124();
void Func_02002144();
void Func_0200222a();
void Func_02002240();
s32 Func_020023e8();
void Func_02002416();
void Func_02002418();
s32 Func_02002420();
s32 Func_0200243a();
void Func_02002442();
void Func_0200246c();
void Func_02002476();
void Func_02002480();
void Func_0200248a();
void Func_02002490();
s32 Func_02002496();
void Func_020024a8();
s32 Func_020024b0();
void Func_020024b4();
void Func_020024c0();
s32 Func_020024c6();
void Func_020024ca();
void Func_020024d8();
s32 Func_020024e0();
void Func_020024f0();
void Func_020024f2();
void Func_020024fa();
void Func_02002512();
void Func_02002516();
void Func_0200252a();
void Func_02002542();
void Func_02002556();
void Func_020025b2();
void Func_020025c6();
void Func_020025cc();
void Func_020025d8();
void Func_020025e2();
void Func_020025ee();
void Func_020025f0();
void Func_020025fc();
void Func_02002600();
void Func_02002608();
void Func_0200260e();
void Func_02002610();
void Func_02002620();
void Func_02002630();
void Func_0200263a();
void Func_0200263c();
void Func_0200266a();
void Func_02002680();
void Func_02002694();
void Func_02002698();
void Func_0200269c();
void Func_020026a4();
void Func_020026b4();
void Func_020026b6();
void Func_020026b8();
void Func_020026bc();
void Func_020026c0();
void Func_020026ca();
void Func_020026d4();
void Func_02002704();
void Func_02002714();
void Func_02002718();
void Func_0200271a();
void Func_02002720();
void Func_0200273a();
void Func_02002744();
void Func_0200274e();
void Func_02002758();
void Func_02002762();
void Func_02002764();
void Func_02002770();
void Func_02002772();
void Func_02002782();
void Func_0200278a();
void Func_0200278e();
void Func_02002792();
void Func_0200279a();
void Func_0200279c();
void Func_020027a2();
s32 Func_020027a6();
void Func_020027b2();
void Func_020027d2();
void Func_020027e4();
void Func_020027ec();
s32 Func_020027ee();
void Func_020027f8();
void Func_020027fe();
void Func_02002806();
void Func_0200280c();
void Func_02002812();
void Func_0200281a();
void Func_0200281e();
void Func_02002820();
void Func_02002822();
void Func_0200283a();
void Func_02002844();
void Func_02002854();
void Func_02002856();
void Func_02002864();
void Func_02002878();
void Func_0200287c();
void Func_02002882();
void Func_02002884();
void Func_02002890();
void Func_020028bc();
void Func_020028c6();
void Func_020028d6();
void Func_020028e4();
void Func_02002900();
void Func_0200290a();
void Func_02002916();
void Func_02002918();
void Func_02002922();
void Func_0200293c();
s32 Func_02002944();
void Func_0200297e();
void Func_02002984();
void Func_020029a6();
void Func_020029ba();
void Func_020029bc();
void Func_020029be();
void Func_020029d0();
void Func_020029d2();
void Func_020029d4();
void Func_020029d6();
void Func_020029e0();
void Func_020029e2();
void Func_020029e6();
void Func_020029e8();
void Func_020029ea();
void Func_020029ee();
void Func_020029f0();
void Func_020029f6();
void Func_020029f8();
s32 Func_020029fa();
void Func_02002a00();
void Func_02002a06();
void Func_02002a08();
void Func_02002a0a();
void Func_02002a20();
void Func_0200962d();

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

void FieldScene_RunFourActorEncounter(void)
{
    u32 i;
    s32 rec;
    s32 record;
    u8 *work;
    s32 v6;
    s32 v5;
    s32 base5_200962d;
    s32 base5_2009ec8;

    rec = Value1(Func_020023e8, 3);
    *(u8 *)(Func_02002420(3) + 35) &= 254;
    Func_020024b4(3, 2);
    *(u8 *)(Func_0200243a(0) + 35) &= 254;
    Func_020024ca(0, 2);
    Func_02002418(184);
    Func_02002556(17);
    Func_02002442();
    Call3(Func_0200246c, 0, 0xcccc, 0x6666);
    Call3(Func_02002476, 1, 0xcccc, 0x6666);
    Call3(Func_02002480, 2, 0xcccc, 0x6666);
    Call3(Func_0200248a, 3, 0xcccc, 0x6666);
    Call3(Func_020024b0, 0, 0xa60000, 0x500000);
    v6 = 192;
    record = Func_02002496(0);
    *(volatile u16 *)(record + 6) = (v6 << 8);
    Call3(Func_020024ca, 1, 0x940000, 0x5a0000);
    record = Func_020024b0(1);
    *(volatile u16 *)(record + 6) = (v6 << 8);
    Call3(Func_020024e0, 2, 0xb60000, 0x5a0000);
    record = Value1(Func_020024c6, 2);
    *(volatile u16 *)(record + 6) = (v6 << 8);
    if (rec != 0) {
        Call3(Func_020024fa, 3, 0xa60000, 0x680000);
        record = Value1(Func_020024e0, 3);
        *(volatile u16 *)(record + 6) = (v6 << 8);
    }
    Func_02001e14(0);
    Func_02002416(10);
    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0x100;
    *(volatile s32 *)(((s32)work + 0x1c8)) = 48;
    Func_020025f0();
    Func_020025fc();
    Func_020024f2(20);
    Call2(Func_020025b2, 0x13333, 0x2666);
    Call4(Func_020025cc, 0xa80000, -1, 0x980000, 1);
    Func_020025d8();
    Func_02002516(10);
    v5 = 10;
    Func_0200263c(123);
    Call6(Func_020024f0, 26, 3, 1, 2, v5, 8);
    Call6(Func_020024fa, 26, 38, 1, 1, v5, 43);
    Func_02002490(4);
    Call6(Func_02002512, 26, 37, 1, 2, v5, 42);
    Func_020024a8(4);
    Call6(Func_0200252a, 26, 36, 1, 3, v5, 41);
    Func_020024c0(4);
    Call6(Func_02002542, 26, 35, 1, 4, v5, 40);
    Func_020024d8(80);
    Call1(Func_0200260e, 0x14d3);
    Call3(Func_02002630, 0x8009, 0, 20);
    Func_02002600(0, 2);
    Func_02002608(1, 2);
    Func_02002610(3, 2);
    Func_02002620(2, 2);
    Func_020025c6(20);
    Call4(Func_02002698, 0xa80000, -1, 0x5a0000, 1);
    Func_020026a4();
    Func_020025e2(40);
    Func_02001f34(1);
    Func_020025ee(60);
    Func_02002714(21);
    Func_02001f46(4);
    Call3(Func_02002694, 0x8009, 0, 20);
    Call3(Func_020026b6, 0, 0x101, 0);
    Call3(Func_020026c0, 1, 0x101, 0);
    Call3(Func_020026ca, 3, 0x101, 0);
    Call3(Func_020026d4, 2, 0x101, 80);
    Call2(Func_020026bc, 0x8009, 0);
    Func_0200263a(40);
    Call3(Func_020026d4, 0x8009, 0, 20);
    Func_02002694(0, 3);
    Func_0200269c(1, 3);
    Func_020026a4(3, 3);
    Func_020026b4(2, 3);
    Func_0200266a(20);
    Call3(Func_02002704, 0x8009, 0, 20);
    Func_02001fc6(0);
    Func_02002680(40);
    Call3(Func_0200271a, 0x8009, 0, 20);
    Call2(Func_02002744, 0, 0x102);
    Call2(Func_0200274e, 1, 0x102);
    Call2(Func_02002758, 3, 0x102);
    Call2(Func_02002762, 2, 0x102);
    Func_020026b8(60);
    Func_02002720(1, 2);
    Call3(Func_02002764, 1, 0xe000, 10);
    Call3(Func_02002770, 0, 0x6000, 10);
    Call3(Func_02002772, 0x8001, 0, 10);
    Func_0200273a(2, 4);
    Call3(Func_0200278e, 0, 0x2000, 0);
    Call3(Func_0200279a, 2, 0xa000, 0);
    Call3(Func_0200279c, 0x8002, 0, 20);
    Func_0200205e(0);
    Func_02002718(40);
    Call3(Func_020027b2, 0x8009, 0, 10);
    Func_02002782(0, 2);
    Func_0200278a(1, 2);
    Func_02002792(3, 2);
    Func_020027a2(2, 2);
    Func_020027e4(0, (v6 << 8), 0);
    ((void (*)())Func_020027ee)(1, (v6 << 8), 0);
    Func_020027f8(2, (v6 << 8), 40);
    Func_020020b2(4);
    Value2(Func_020027ee, 0x8009, 0);
    Call3(Func_02002812, 1, 0xe000, 0);
    Call3(Func_0200281e, 2, 0xa000, 0);
    if (Value2(Func_020027a6, 0, 0) != 0) {
        Call3(Func_02002844, 1, 0x103, 20);
        Func_020027ec(1, 4);
        Call1(Func_02002822, 0x14dd);
        Call2(Func_0200283a, 0x8001, 0);
        Call3(Func_02002864, 2, 0x103, 10);
        Func_0200280c(2, 3);
        Call2(Func_02002854, 0x8002, 0);
    }
    Func_020027d2(20);
    Func_02002124(4);
    Call1(Func_02002856, 0x14df);
    Call3(Func_02002878, 0x8009, 0, 20);
    Call3(Func_02002882, 0x8009, 0, 10);
    Func_02002144(0);
    Func_020027fe(20);
    Call2(Func_02002900, 0x10000, 0);
    Call2(Func_02002900, 0x406218, 1);
    Func_02002916(20);
    Func_02002764(40);
    Func_0200287c(0, 2);
    Func_02002884(1, 2);
    Func_020028c6(3, 2);
    Func_020028d6(2, 2);
    Func_02002918(1, (v6 << 8), 0);
    Func_02002922(2, (v6 << 8), 20);
    Func_02002890(20);
    *(volatile s32 *)Data_0200adcc = 0;
    {
        volatile s32 *bank = (volatile s32 *)Data_0200adc0;
        bank[0] = 0xa80000;
        bank[1] = 0x200000;
        base5_200962d = (s32)Func_0200962d;
        bank[2] = 0x340000;
    }
    Call2(Func_02002806, base5_200962d, 0xc80);
    Func_020028bc(220);
    Func_0200281a(base5_200962d);
    Call2(Func_020029bc, 0x10000, 1);
    Func_020029d2(20);
    Func_02002820(40);
    Func_0200222a(4);
    Func_020028e4(20);
    Call3(Func_0200297e, 0x8009, 0, 10);
    Func_02002240(0);
    Call2(Func_02002984, 0x8009, 0);
    Call2(Func_0200293c, 8, 0x2009e54);
    Func_0200290a(40);
    Call3(Func_020029be, 1, 0x102, 60);
    Call2(Func_020029a6, 0x8001, 0);
    Call3(Func_020029d2, 2, 0x102, 10);
    Call2(Func_020029ba, 0x8002, 0);
    Call3(Func_020029d6, 1, 0xe000, 0);
    Call3(Func_020029e2, 2, 0xa000, 10);
    Call3(Func_020029ee, 0, 0x4000, 10);
    Func_020029be(1, 1);
    Call3(Func_020029f8, 0x8001, 0, 10);
    Func_020029d0(2, 1);
    Call3(Func_02002a0a, 0x8002, 0, 10);
    if (rec != 0) {
        Func_020029e6(3, 1);
        Call3(Func_02002a20, 0x8003, 0, 10);
    }
    Func_020029e0(0, 3);
    Func_020029e8(1, 3);
    Func_020029f0(3, 3);
    Func_02002a00(2, 3);
    base5_2009ec8 = (s32)Data_02009ec8;
    Func_020029ea(1, base5_2009ec8);
    if (rec != 0) {
        Func_020029f6(3, base5_2009ec8);
    }
    Func_02002a06(2, base5_2009ec8);
    Func_020029d4(20);
    *(u8 *)(Func_020029fa(0) + 35) |= 1;
    Call1(Func_020029e2, 0x844);
    Value2(Func_02002944, 0x2009245, 0xc80);
    Func_02002a08();
}
