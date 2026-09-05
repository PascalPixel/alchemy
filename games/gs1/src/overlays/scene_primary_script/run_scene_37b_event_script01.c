#include "types.h"

#define RunEventScript01 Func_02000150

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_0200253e();
void Func_0200259c();
void Func_020025a8();
void Func_020025c4();
void Func_020025d8();
void Func_0200262a();
void Func_0200262c();
void Func_0200263c();
void Func_02002642();
void Func_0200264a();
void Func_0200264c();
void Func_02002652();
void Func_02002664();
void Func_0200266e();
void Func_02002686();
void Func_020026a0();
void Func_020026a6();
void Func_020026ae();
void Func_020026b2();
void Func_020026c0();
void Func_020026c2();
void Func_020026c4();
void Func_020026c6();
void Func_020026da();
void Func_020026dc();
void Func_020026ea();
void Func_020026fa();
void Func_02002700();
void Func_02002704();
void Func_02002706();
void Func_02002710();
void Func_02002722();
void Func_0200272c();
void Func_02002732();
void Func_02002734();
void Func_0200273a();
void Func_0200273e();
void Func_0200274c();
void Func_02002750();
void Func_02002758();
void Func_02002762();
void Func_02002766();
void Func_02002772();
void Func_0200277c();
void Func_02002798();
void Func_020027a4();
void Func_020027a6();
void Func_020027ba();
void Func_020027c2();
void Func_020027c8();
void Func_020027e8();
void Func_020027f2();
void Func_020027f6();
void Func_0200280a();
void Func_02002812();
void Func_0200281c();
void Func_0200281e();
void Func_0200282c();
void Func_0200283a();
void Func_02002840();
void Func_02002848();
void Func_0200284a();
void Func_02002850();
void Func_02002858();
void Func_0200285a();
void Func_0200285e();
void Func_02002868();
void Func_0200286a();
void Func_0200287a();
void Func_02002880();
void Func_02002886();
void Func_0200288a();
void Func_0200288c();
void Func_0200289a();
void Func_020028bc();
void Func_020028be();
void Func_020028c4();
void Func_020028ca();
void Func_020028e2();
void Func_020028e6();
void Func_020028f0();
void Func_020028fa();
void Func_020028fc();
void Func_0200290c();
void Func_0200291e();
void Func_0200292c();
void Func_02002934();
void Func_0200293a();
void Func_02002944();
void Func_0200294e();
void Func_02002952();
void Func_0200295a();
void Func_02002966();
void Func_02002974();
void Func_0200297c();
void Func_0200298e();
void Func_02002998();
void Func_020029a8();
void Func_020029aa();
void Func_020029b8();
void Func_020029c0();
void Func_020029c2();
void Func_020029dc();
void Func_020029f8();
s32 Func_02002a06();
void Func_02002a20();
void Func_02002a28();
s32 Func_02002a36();
void Func_02002a3a();
void Func_02002a46();
void Func_02002a4e();
void Func_02002a58();
void Func_02002a60();
void Func_02002a64();
void Func_02002a70();
void Func_02002a7e();
void Func_02002a90();
extern u8 Data_03001ebc[];

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

void RunEventScript01(void)
{
    u32 i;
    s32 record;

    Func_020025a8();
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x100;
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c8) = 32;
    Func_020026b2();
    Func_020026c6();
    Func_020025c4(20);
    Call3(Func_0200262a, 8, 0x2400000, 0xe80000);
    Func_020025d8(1);
    Call1(Func_0200266e, 0x101a);
    Func_0200253e(8, 6);
    Call3(Func_0200264c, 8, 0x2400000, 0x1180000);
    Call2(Func_020026c4, 0xcccc, 0x1999);
    Call4(Func_020026dc, 0x23e0000, -1, 0xb40000, 1);
    Call3(Func_02002642, 8, 0x10000, 0x8000);
    Call3(Func_0200266e, 8, 0x240, 216);
    Func_0200262c(20);
    Func_020026a6(5, 2, 0);
    Func_0200263c(30);
    Func_0200259c(5, 6);
    Func_020026c4(8, 2);
    Func_02002652(6);
    Call3(Func_02002706, 8, 0x9000, 0);
    Func_02002664(10);
    Call2(Func_02002734, 0x59999, 0xb333);
    Call4(Func_0200274c, 0x11f0000, -1, 0xb00000, 1);
    Func_02002758();
    Func_02002686(60);
    Call4(Func_02002766, 0x23e0000, -1, 0xb40000, 1);
    Func_02002772();
    Func_020026a0(20);
    Func_02002710(8, 3);
    Func_020026ae(10);
    Call3(Func_02002762, 8, 0xc000, 0);
    Func_020026c0(10);
    Func_0200273a(8, 6, 0);
    Call3(Func_02002700, 8, 0x30000, 0x20000);
    Call3(Func_0200272c, 8, 0x240, 184);
    Func_020026ea(40);
    Func_0200264a(8, 6);
    Call3(Func_020027a6, 8, 0x8000, 0);
    Func_02002704(40);
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c0) = 0x202;
    *(s32 *)(*(u8 **)Data_03001ebc + 0x1c8) = 16;
    Func_02002812();
    Func_0200281e();
    Call4(Func_020027f6, 0x11f0000, -1, 0xb00000, 0);
    Func_020026da();
    Func_020026a0(1);
    Func_0200282c();
    Func_02002840();
    Func_0200273e(40);
    Call3(Func_020027a4, 8, 0x1a80000, 0xc80000);
    Func_020026c2(1);
    Func_020026b2(8, 20);
    Func_0200285e();
    Func_0200286a();
    Call3(Func_020027c8, 8, 0x2400000, 0xb80000);
    Call4(Func_02002850, 0x23e0000, -1, 0xb40000, 0);
    Func_02002734();
    Func_020026fa(1);
    Func_02002886();
    Func_0200289a();
    Func_02002798(20);
    Call2(Func_02002868, 0x13333, 0x2666);
    Call4(Func_02002880, 0x23e0000, -1, 0x9d0000, 1);
    Func_0200288c();
    Func_020027ba(20);
    Func_0200283a(1, 2);
    Func_02002722(1, 20);
    Func_0200283a(5, 3);
    Func_02002732(5, 6);
    Func_0200285a(8, 2);
    Func_020027e8(40);
    Func_02002858(8, 4);
    Func_02002750(8, 80);
    Call3(Func_020028bc, 8, 0x102, 0);
    Func_0200280a(60);
    Call3(Func_020028be, 8, 0xc000, 0);
    Func_0200281c(30);
    Func_0200277c(8, 20);
    Call3(Func_020028e6, 0, 0x101, 0);
    Call3(Func_020028f0, 1, 0x101, 0);
    Call3(Func_020028fa, 5, 0x101, 0);
    Func_02002848(60);
    Call3(Func_020028fc, 8, 0xc000, 0);
    Func_0200285a(30);
    Func_020028ca(8, 4);
    Func_020027c2(8, 6);
    Func_020028fc(0, 1, 0);
    Func_0200287a(40);
    Func_0200290c(0, 5, 0);
    Func_0200288a(40);
    Func_020028fa(8, 3);
    Func_020027f2(8, 6);
    Call3(Func_0200294e, 0, 0x4000, 0);
    Call3(Func_0200295a, 5, 0x4000, 0);
    Call3(Func_02002966, 1, 0x4000, 0);
    Func_020028c4(40);
    Func_0200292c(1, 3);
    Func_02002934(5, 3);
    Func_02002944(0, 3);
    Func_020028e2(20);
    Func_02002952(8, 3);
    Func_0200284a(8, 6);
    Call2(Func_020029c2, 0x19999, 0x3333);
    Call4(Func_020029dc, 0x2400000, -1, 0xd70000, 1);
    Call3(Func_020029c0, 8, 0x4000, 0);
    Func_0200291e(10);
    Func_02002998(8, 6, 0);
    Call3(Func_0200297c, 8, 0x240, 217);
    Func_0200293a(20);
    Call3(Func_0200298e, 8, 0x240, 0x141);
    Func_020029a8(8, 0, 0);
    Call2(Func_02002a20, 0x39999, 0x7333);
    Call4(Func_02002a3a, 0x2400000, -1, 0x880000, 1);
    Func_02002a46();
    Func_02002974(20);
    Call3(Func_020029aa, 1, 0x10000, 0x8000);
    Call3(Func_020029b8, 5, 0x10000, 0x8000);
    Func_020029f8(1, 2);
    record = Value1(Func_02002a06, 0);
    if (record != 0) {
        Func_02002a28(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02002a4e(1);
    Func_02002a60(1, 0, 0);
    Func_02002a70(5, 2);
    record = Value1(Func_02002a36, 0);
    if (record != 0) {
        Func_02002a58(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02002a7e(5);
    Func_02002a90(5, 0, 0);
    Call1(Func_02002a36, 0x12f);
    *(s32 *)((*(u8 **)Data_03001ebc + 0x1c0)) = 0x204;
    *(s32 *)((*(u8 **)Data_03001ebc + 0x1c8)) = 16;
    Func_02002a64();
}
