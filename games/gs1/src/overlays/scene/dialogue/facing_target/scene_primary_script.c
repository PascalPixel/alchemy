#include "types.h"

#define FieldScene_RunScene377_020001e0 Func_020001e0
#define FieldScene_RunStep7BThen2 Func_02000330
#define FieldScene_RunStep7BThen4 Func_02000358
#define FieldScene_RunStep80Then5 Func_0200036c
#define FieldScene_RunStep7BThen6 Func_02000380
#define FieldScene_RunStep80Then7 Func_02000394
#define FieldScene_RunStep7BThen10 Func_020003d0
#define FieldScene_RunScene377_02000e34 Func_02000e34
#define FieldScene_RunSupplementalSequenceOne Func_020013e4
#define FieldScene_RunStep17 Func_020017d4
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];

void Func_02001af0();
void Func_02001af6();
void Func_02001b2a();
void Func_02001b38();
s32 Func_02001b68();
void Func_02001b6a();
void Func_02001b80();
void Func_02001b88();
void Func_02001bae();
void Func_02001bb2();
void Func_02001bb4();
void Func_02001bbc();
void Func_02001be8();
void Func_02001bf4();
s32 Func_02001c08();
void Func_02001c1c();
void Func_02001c4c();
void Func_02001de2(s32);
void Func_0200061c(s32);
void Func_02001e0a(s32);
void Func_02000644(s32);
void Func_02001e1e(s32);
void Func_02000658(s32);
void Func_02001e32(s32);
void Func_0200066c(s32);
void Func_02001e46(s32);
void Func_02000680(s32);
void Func_02001e82(s32);
void Func_020006bc(s32);
s32 Func_02002748();
void Func_02002762();
s32 Func_02002766();
s32 Func_02002766_a();
void Func_02002788();
s32 Func_020027a0();
void Func_020027e6();
void Func_02002804();
void Func_02002816();
void Func_02002848();
void Func_0200285e();
void Func_02002860();
void Func_02002882();
s32 Func_02002886();
void Func_0200288a();
void Func_02002898();
void Func_020028a2();
void Func_020028f4();
void Func_02002c18();
void Func_02002c36();
s32 Func_02002c98();
s32 Func_02002c9e();
void Func_02002cc4();
void Func_02002cce();
void Func_02002cf8();
void Func_02002d3c();
void Func_02002d4a();
void Func_02002d5e();
void Func_02002d62();
void Func_02002d6c();
void Func_02002d70();
void Func_02002d82();
void Func_02002d90();
void Func_02002de4();
void Func_02002e06();
void Func_02002e22();
void Func_02002e28();
void Func_02002e34();
void Func_02002e3e();
void Func_02002e40();
void Func_02002e9e();
void Func_02002ecc();
void Func_02002efe();
void Func_02002f08();
void Func_02002f5e();
void Func_02002f84();
void Func_02002f90();
void Func_02003106(s32);
void Func_02002d9a(void);

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

void FieldScene_RunScene377_020001e0(void)
{
    u32 i;
    s32 record;

    Func_02001af0();
    Func_02001b80(16, 2);
    Func_02001af6(30);
    Call1(Func_02001bbc, 0xf5b);
    Func_02001bae(0, 16, 10);
    Func_02001be8(16, 0, 6);
    Call3(Func_02001c1c, 16, 0x102, 0);
    Func_02001bb4(16, 1);
    Func_02001b2a(20);
    Func_02001bb2(16, 4);
    Func_02001b38(20);
    Value2(Func_02001c08, 16, 0);
    if (Value2(Func_02001b68, 0, 0) == 1) {
        bump_step(1);
    }
    Func_02001bf4(16, 1);
    Func_02001b6a(20);
    Func_02001c4c(16, 0, 4);
    Func_02001b88();
}

void FieldScene_RunStep7BThen2(void)
{
    Func_02001de2(0x7B);
    Func_0200061c(2);
}

void FieldScene_RunStep7BThen4(void)
{
    Func_02001e0a(0x7B);
    Func_02000644(4);
}

void FieldScene_RunStep80Then5(void)
{
    Func_02001e1e(0x80);
    Func_02000658(5);
}

void FieldScene_RunStep7BThen6(void)
{
    Func_02001e32(0x7B);
    Func_0200066c(6);
}

void FieldScene_RunStep80Then7(void)
{
    Func_02001e46(0x80);
    Func_02000680(7);
}

void FieldScene_RunStep7BThen10(void)
{
    Func_02001e82(0x7B);
    Func_020006bc(10);
}

void FieldScene_RunScene377_02000e34(void)
{
    u32 i;
    s32 record;

    record = Func_02002766(0);
    if ((u32)(*(volatile u16 *)(record + 6) + -0x2000) > 0x9000) {
        Func_020028f4(0, 13);
    } else {
        Func_02002762();
        if (Value1(Func_02002748, 0x87a) != 0) {
            Func_02002804(13, 2);
            Func_02002816(13, 0, 10);
            if (Value1(Func_02002766_a, 0x300) == 0) {
                Call1(Func_02002848, 0x1c14);
                Func_02002860(13, 0);
                Call1(Func_02002788, 0x300);
            }
            Call1(Func_0200285e, 0x1c15);
            Value2(Func_02002886, 13, 0);
            Func_02002898(13, 0x9000, 10);
        } else {
            if (Value1(Func_020027a0, 0x815) != 0) {
                Call1(Func_02002882, 0x11a9);
            } else {
                Call1(Func_0200288a, 0xf58);
            }
            Func_020028a2(13, 0);
        }
        Func_020027e6();
    }
}

void FieldScene_RunSupplementalSequenceOne(s32 a0)
{
    u32 i;
    u8 *rec2;
    s32 rec7;
    s32 record;
    u8 *p6;

    Func_02002cf8(a0);
    Call4(Func_02002e22, -1, -1, -1, 0);
    Func_02002c18(1);
    Func_02002d82(0, 0, 0);
    Call3(Func_02002d90, 18, 0x1e00000, 0xca0000);
    Func_02002c36(1);
    Func_02002e3e(18, 1);
    rec7 = 0;
    rec2 = Value4(Func_02002c98, 22, 0x1480000, 0x20000, 0xc30000);
    rec2[85] = rec7;
    p6 = *(volatile s32 *)((s32)rec2 + 80);
    *(volatile s32 *)((s32)rec2 + 12) = 0x50000;
    p6[39] = rec7;
    {
        s32 mask = -33;
        p6[5] &= mask;
    }
    p6[9] &= 15;
    rec7 = Value2(Func_02002c9e, 17, 0x608);
    Func_02002d5e(224);
    Func_02002cce(p6[28], 128, (rec7 + 0x400));
    Func_02002cc4(17);
    {
        s32 off = 0x1c0;
        u8 *base = *(u8 *volatile *)Data_03001ebc;
        s32 *slot = (s32 *)(base + off);
        off += 66;
        *slot = off;
    }
    Func_02002efe();
    Call3(Func_02002de4, 18, 0x10000, 0x8000);
    Call3(Func_02002e28, 18, 0x1e0, 176);
    Call3(Func_02002e34, 18, 0x1a4, 164);
    Call3(Func_02002e40, 18, 0x146, 185);
    Call3(Func_02002ecc, 18, 0x4000, 10);
    Func_02002d3c((s32)rec2, 0x2009eac);
    Func_02002d62((s32)rec2);
    Call2(Func_02002d4a, (s32)rec2, 0x2009ecc);
    Func_02002d70((s32)rec2);
    Func_02002e06(20);
    Func_02002d6c((s32)rec2);
    Func_02002e9e(18, 2, 20);
    Func_02002f08(18, 0, 40);
    Func_02002f84();
    Func_02002f90();
    Func_02002f5e(22);
}

void FieldScene_RunStep17(void)
{
    Func_02003106(17);
    Func_02002d9a();
}
