#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define FieldScene_RunScene39b_02000cc0 Func_02000cc0
#define FieldScene_RunScene39b_02000f48 Func_02000f48
#define FieldScene_RunSupplementalSequenceTwo Func_02000fd8
#define FieldScene_RunScene39b_0200116c Func_0200116c
#define FieldScene_RunScene39b_02001208 Func_02001208
#define FieldScene_RunScene39b_0200196c Func_0200196c

extern u8 Data_03001ebc[];

void Func_02000e6a();
s32 Func_020030ea();
void Func_0200340a();
void Func_0200342a();
void Func_02003452();
void Func_02003458();
s32 Func_02003464();
void Func_02003470();
void Func_0200348e();
void Func_020034c2();
void Func_020034d2();
void Func_02003568();
void Func_020011e8();
s32 Func_0200345a();
s32 Func_02003466();
s32 Func_0200346c();
s32 Func_02003474();
s32 Func_020034cc();
s32 Func_020034d2_a();
s32 Func_020034e4();
void Func_020034f0();
void Func_020034fe();
void Func_02003512();
void Func_02003560();
void Func_0200357c();
void Func_02003586();
void Func_02003592();
void Func_0200359c();
void Func_020035da();
void Func_020035ea();
void Func_020035ee();
void Func_020035f0();
void Func_020035fe();
void Func_02003604();
void Func_0200366e();
void Func_020036c6();
void Func_020036d6();
void Func_020036dc();
s32 Func_0200365e();
void Func_02003648();
void Func_0200368c();
void Func_02003696();
s32 Func_020036d2();
void Func_020036ee();
s32 Func_020036fa();
void Func_020036fc();
void Func_02003728();
s32 Func_02003748();
void Func_02003772();
s32 Func_0200377a();
s32 Func_02003782();
void Func_020037a8();
void Func_020037aa();
void Func_020037c2();
void Func_020037e4();
void Func_0200383a();
void Func_0200384e();
void Func_02003884();
void Func_02003884_a();
void Func_020038b2();
void Func_020038cc();
s32 Func_02003e30();
void Func_02003e44();
void Func_02003e60();
s32 Func_02003e76();
s32 Func_02003e80();
s32 Func_02003e88();
void Func_02003ea6();
void Func_02003ebe();
void Func_02003ece();
void Func_02003ed8();
void Func_02003edc();
void Func_02003f00();
void Func_02003f08();
void Func_02003f1c();
void Func_02003f20();
void Func_02003f28();
void Func_02003f2c();
void Func_02003f3e();
void Func_02003f46();
void Func_02003f9c();
void Func_02003fb0();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

/* A value-returning call sets r0 last of its arguments. */
static __inline__ s32 Value1_0200116c(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02001208(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3_02001208(void (*f)(), s32 a0, s32 a1, s32 a2)
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

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_0200196c(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3_0200196c(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

s32 FieldScene_RunScene39b_02000cc0(s32 a0)
{
    s32 value;
    s32 magic;
    s32 base5_3001e40;
    s32 none;
    u8 storage[40];
    u8 *rec = storage;

    FIELD(rec, s32, 4) = 7;
    base5_3001e40 = 0x3001e40;
    if ((*(volatile s32 *)base5_3001e40 & 1) == 0) {
        FIELD(rec, s32, 4) = 5;
    }
    FIELD(rec, s32, 8) = 0xcccc;
    FIELD(rec, s32, 12) = 0xcccc;
    none = 0;
    FIELD(rec, s32, 0) = none;
    value = Func_020030ea();
    magic = -((((((u32)(value << 3) >> 16) << 1) + ((u32)(value << 3) >> 16)) + (((((u32)(value << 3) >> 16) << 1) + ((u32)(value << 3) >> 16)) << 4)) + ((((((u32)(value << 3) >> 16) << 1) + ((u32)(value << 3) >> 16)) + (((((u32)(value << 3) >> 16) << 1) + ((u32)(value << 3) >> 16)) << 4)) << 8));
    Func_02000e6a((*(volatile s32 *)(a0 + 8) + ((8 - (*(volatile s32 *)base5_3001e40 & 15)) << 16)), (*(volatile s32 *)(a0 + 12) + 0x1a0000), *(volatile s32 *)(a0 + 16), none, magic, 0, 0xb0000, rec);
    return 0;
}

void FieldScene_RunScene39b_02000f48(s32 a0)
{
    u32 i;
    s32 record;

    Func_0200342a();
    Func_02003568(228);
    Call3(Func_02003452, 0, 0x6666, 0x3333);
    Func_020034d2(0, 2);
    Call3(Func_0200348e, 0, 0, -8);
    record = Func_02003464(0);
    Func_0200340a(record, 0);
    Func_02003458(8);
    Func_020034c2(0, ((a0 << 19) + 0x80000), 0);
    Func_02003470(30);
}

void FieldScene_RunSupplementalSequenceTwo(void)
{
    s32 a;
    s32 b;
    s32 zero;
    s32 counter;
    s32 x;
    s32 y;
    s32 t;
    s32 record;
    u8 *slot;
    u8 slot16[40];

    a = *(volatile s32 *)(Value1(Func_020034d2_a, 0) + 8) / 0x100000;
    b = *(volatile s32 *)(Value1(Func_020034e4, 0) + 16) / 0x100000;
    if (a == 12 && b == 32) {
        Func_020034f0();
        Call2(Func_020035da, 0x10000, 0);
        Func_020035f0(60);
        Func_020034fe(120);
        Call2(Func_020035ee, 0x10005, 1);
        Func_02003604(60);
        Func_02003512(40);
        counter = 0;
        slot = slot16;
        zero = 0;
        do {
            *(s32 *)(slot) = 1;
            {
                s32 shown = 0x11e;

                *(u16 *)(slot + 24) = shown;
            }
            *(s32 *)(slot + 28) = 0x200af4c;
            Func_0200366e(246);
            x = 208 - ((u32)(Func_0200345a() << 4) >> 16);
            y = 560 - ((u32)(Func_02003466() << 4) >> 16);
            t = ((u32)(Value0(Func_02003474) << 2) >> 16);
            record = Func_0200346c((((t << 4) - t) << 16) + 0x3c0000, 100);
            Func_020011e8(x << 16, 0, y << 16, 0, record, zero, 0x320001, slot);
            Func_02003586(4);
            counter = counter + 1;
        } while ((u32)counter <= 14);
        Func_020036d6(220);
        Func_0200359c(60);
        Call1(Func_02003592, 0x875);
        Value2(Func_020034cc, 0x2008d99, 0xc80);
        Call6(Func_02003560, 37, 98, 10, 97, 5, 3);
        Call6(Func_0200357c, 70, 32, 13, 7, 6, 32);
        Call2(Func_020036c6, 0x10000, 0);
        Func_020036dc(60);
        Func_020035ea(120);
        Func_020035fe();
    }
}

void FieldScene_RunScene39b_0200116c(void)
{
    s32 record;
    s32 field8;
    s32 quotient;

    record = Value1_0200116c(Func_0200365e, 0);
    field8 = *(s32 *)(record + 8);
    quotient = field8 / 0x100000;
    Func_02003648(0x205);
    if (quotient == 7) {
        Func_0200368c(8, 0x200a874);
    } else {
        Func_02003696(8, 0x200a8c8);
    }
}

void FieldScene_RunScene39b_02001208(void)
{
    u32 i;
    u8 *rec7;
    s32 rec8;
    s32 record;

    rec7 = Value1_02001208(Func_020036fa, 0);
    rec8 = Value1_02001208(Func_020036d2, 0x109);
    if (rec8 == 0) {
        Func_020036fc();
        Call4(Func_020037c2, -1, -1, -1, 0);
        rec7[85] = rec8;
        Value3(Func_0200377a, 0, (*(s16 *)((s32)rec7 + 10) << 16), ((*(s16 *)((s32)rec7 + 18) << 16) + -0x100000));
        Func_020037aa(0, 15);
        record = Func_02003748(0);
        Func_020036ee(record, 0);
        Func_0200383a();
        Func_0200384e();
        Func_02003884(228);
        *(volatile s32 *)((s32)rec7 + 108) = 0x2008cc1;
        Call3_02001208(Func_02003772, 0, 0x6666, 0x3333);
        Func_02003884_a(0, 0, 8);
        Func_020037e4(0, 0);
        record = Func_02003782(0);
        Func_02003728(record, 1);
        {
            u8 *rec9 = (u8 *)*(volatile s32 *)((s32)rec7 + 80);
            s32 mask = -13;
            rec9[9] = (mask & rec9[9]) | 4;
        }
        Func_020038b2(0, 0, 10);
        rec7[85] = 3;
        *(volatile s32 *)((s32)rec7 + 108) = rec8;
        Func_020038cc();
        Func_020037a8();
    }
}

void FieldScene_RunScene39b_0200196c(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02003e30, 0x250) == 0) {
        Call1_0200196c(Func_02003e44, 0x250);
        Func_02003e60();
        record = Func_02003e76(12);
        *(volatile s32 *)(record + 24) = -0x10000;
        record = Value1(Func_02003e80, 13);
        *(volatile s32 *)(record + 24) = -0x10000;
        record = Func_02003e88(14);
        *(volatile s32 *)(record + 24) = -0x10000;
        Call3_0200196c(Func_02003ed8, 3, 0x880000, 0x900000);
        Call3_0200196c(Func_02003f1c, 3, 0x4000, 10);
        *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0) = 0x201;
        Func_02003f9c();
        Func_02003fb0();
        Func_02003ea6(60);
        Func_02003f28(3, 0, 0);
        Func_02003f20(3, 3);
        Func_02003ebe(30);
        Func_02003f00(3, 136, 72);
        Func_02003ece(40);
        Func_02003f46(0, 1);
        Func_02003f2c(3);
        Func_02003f3e(3, 0, 0);
        Call1_0200196c(Func_02003edc, 0x872);
        *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0) = 0x204;
        Func_02003f08();
    }
}
