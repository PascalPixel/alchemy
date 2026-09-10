#include "types.h"

#define FieldScene_RunPrimarySequence Func_020005f4
#define FieldScene_RunScene3a0_02000968 Func_02000968
#define FieldScene_RunScene3a0_02000de8 Func_02000de8
#define FieldScene_RunScene3a0_02001060 Func_02001060
extern u8 Data_03001ebc[];

s32 Func_020019b4();
void Func_020019c2();
s32 Func_020019cc();
u8 *Func_020019d8();
s32 Func_020019ec();
s32 Func_020019ee();
void Func_020019f6();
u8 *Func_020019fc();
s32 Func_02001a3e();
s32 Func_02001a48();
u8 *Func_02001a66();
void Func_02001a66_a();
s32 Func_02001a76();
void Func_02001a80();
s32 Func_02001a84();
s32 Func_02001a92();
void Func_02001aa0();
void Func_02001aa4();
void Func_02001ab0();
s32 Func_02001abe();
u8 *Func_02001ac6();
void Func_02001ad0();
s32 Func_02001ad4();
s32 Func_02001ae4();
void Func_02001aec();
s32 Func_02001afe();
s32 Func_02001b0e();
void Func_02001b10();
void Func_02001b24();
u8 *Func_02001b2a();
u8 *Func_02001b2a_a();
s32 Func_02001b36();
void Func_02001b3a();
u8 *Func_02001b3e();
void Func_02001b46();
void Func_02001b4e();
void Func_02001b58();
void Func_02001b66();
s32 Func_02001b70();
void Func_02001b74();
s32 Func_02001b78();
s32 Func_02001b98();
s32 Func_02001ba0();
void Func_02001bb0();
void Func_02001bc8();
void Func_02001be6();
void Func_02001bea();
void Func_02001bfe();
s32 Func_02001c30();
s32 Func_02001c3e();
void Func_02001c4c();
void Func_02001c5e();
s32 Func_02001c68();
void Func_02001c70();
void Func_02001c76();
void Func_02001c78();
s32 Func_02001ca2();
void Func_02001ca4();
void Func_02001ca8();
void Func_02001cac();
void Func_02001cae();
u8 *Func_02001cb0();
void Func_02001cc6();
void Func_02001cc8();
void Func_02001ccc();
u8 *Func_02001cd6();
void Func_02001cda();
void Func_02001cda_a();
void Func_02001cdc();
void Func_02001cf2();
void Func_02001cf4();
void Func_02001d00();
void Func_02001d0a();
void Func_02001d12();
void Func_02001cf8();
void Func_02001d2e();
void Func_02001d2e_a();
s32 Func_02001d44();
s32 Func_02001d54();
s32 Func_02001d60();
void Func_02001d66();
s32 Func_02001d68();
void Func_02001d92();
void Func_02001e4e();
s32 Func_020021c0();
void Func_020021dc();
void Func_02002234();
void Func_0200223a();
void Func_02002240();
void Func_020022d6();
void Func_02002424();
s32 Func_02002442();
s32 Func_0200244c();
s32 Func_0200244c_a();
s32 Func_02002458();
s32 Func_02002460();
s32 Func_02002468();
s32 Func_02002470();
s32 Func_02002478();
void Func_0200247a();
void Func_020024a8();
void Func_020024b6();
void Func_020024ea();
s32 Func_020024f2();
void Func_020024f2_a();
void Func_020024f4();
void Func_020024fc();
void Func_020024fe();
void Func_0200251c();
void Func_02002544();
void Func_02002588();
void Func_020025a4();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
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

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02000968(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02000de8(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void FieldScene_RunPrimarySequence(void)
{
    u32 i;
    s32 rec7;
    s32 rec8;
    u8 *record;
    s32 v7;
    s32 v5;
    s32 p5;
    s32 q1;
    s32 t2;
    s32 q2;
    s32 hi;
    s32 lo;

    rec8 = Value1(Func_020019cc, 20);
    Func_020019c2();
    v7 = 0;
    record = Func_020019d8(18);
    *(volatile s32 *)((s32)record + 108) = v7;
    if (Value1(Func_020019b4, 0x200) == 0) {
        record = Value1(Func_020019ee, 18);
        if ((*(volatile s32 *)((s32)record + 8) >> 20) > 19) {
            goto L_020006a2;
        }
    }
    record = Func_020019fc(18);
    p5 = *(u16 *)((s32)record + 6);
    Func_02001a80(18, 0, 0);
    Func_020019f6(10);
    Call1(Func_02001aa4, 0x17fb);
    if (Value1(Func_020019ec, 0x200) == 0) {
        bump_step(1);
        Func_02001ad0(18, 0);
        *(volatile u16 *)(Func_02001a3e(18) + 100) = v7;
        record = Value1(Func_02001a48, 18);
        *(volatile u16 *)((s32)record + 6) = p5;
    } else {
        Func_02001aec(18, 0);
        Call3(Func_02001b10, 18, 0x8000, 20);
    }
    record = Func_02001a66(18);
    *(volatile s32 *)((s32)record + 108) = 0x2008501;
    Call0(Func_02001a66_a);
    goto L_02000916;
    L_020006a2:;
    record = Value1(Func_02001a76, 0);
    if ((*(volatile s32 *)((s32)record + 16) >> 19) > 27) {
        record = Value1(Func_02001a84, 0);
        if ((*(volatile s32 *)((s32)record + 16) >> 19) <= 29) {
            record = Value1(Func_02001a92, 0);
            if ((*(volatile s32 *)((s32)record + 8) >> 20) != 26) {
                Call3(Func_02001ab0, 0, 0x8000, 0x4000);
                Call3((void (*)())Func_02001b2a, 0, 18, 0);
                Func_02001aa0(5);
                rec7 = Value1(Func_02001abe, 0);
                record = Func_02001ac6(18);
                if (*(volatile s32 *)(rec7 + 8) < *(volatile s32 *)((s32)record + 8)) {
                    *(u8 *)(Func_02001ad4(0) + 90) &= 254;
                    record = Value1(Func_02001ae4, 18);
                    Func_02001b24(0, (((*(volatile s32 *)((s32)record + 8) >> 20) << 4) - 8), 232);
                    v7 = 1;
                } else {
                    *(u8 *)(Func_02001afe(0) + 90) &= 254;
                    record = Value1(Func_02001b0e, 18);
                    Func_02001b4e(0, (((*(volatile s32 *)((s32)record + 8) >> 20) << 4) + 24), 232);
                }
                Func_02001b74(0);
            }
        }
    }
    v5 = 128;
    record = Func_02001b2a_a(18);
    *(volatile s32 *)((s32)record + 56) = (v5 << 24);
    record = Value1(Func_02001b36, 18);
    *(volatile s32 *)((s32)record + 60) = (v5 << 24);
    record = Func_02001b3e(18);
    *(volatile s32 *)((s32)record + 64) = (v5 << 24);
    Func_02001b58(18, 1);
    Func_02001bb0(18, 1);
    Func_02001bc8(18, 2);
    Func_02001b46(10);
    Func_02001c4c(228);
    *(s32 *)(rec8 + 24) = 0x4ccc;
    *(s32 *)(rec8 + 28) = 0x4ccc;
    record = Value1(Func_02001b70, 18);
    q1 = *(volatile s32 *)((s32)record + 8);
    record = Value1(Func_02001b78, 18);
    t2 = *(volatile s32 *)((s32)record + 16) >> 20;
    Func_02001bea(20, (((q1 >> 20) << 20) + 0x80000), ((t2 << 20) + 0x80000));
    record = Value1(Func_02001b98, 18);
    q2 = *(volatile s32 *)((s32)record + 8);
    record = Value1(Func_02001ba0, 18);
    Call6(Func_02001b66, 16, 16, 1, 1, (q2 >> 20), (*(volatile s32 *)((s32)record + 16) >> 20));
    Func_02001c76(20, 2);
    {
        u8 *flags = (u8 *)(rec8 + 35);
        u8 value = *(volatile u8 *)flags;

        *flags = (u8)(value | 2);
    }
    do {
        Func_02001b3a(3);
        hi = *(s32 *)(rec8 + 28);
        lo = *(s32 *)(rec8 + 24);
        *(s32 *)(rec8 + 28) = hi + 0x1999;
        lo += 0x1999;
        *(s32 *)(rec8 + 24) = lo;
    } while (lo <= 0xffff);
    Call3(Func_02001cae, 18, 0x105, 70);
    Func_02001c70(18, 0, 0);
    Func_02001be6(20);
    Call3(Func_02001cc8, 18, 0x103, 0);
    Func_02001c78(18, 2);
    Func_02001bfe(70);
    Call1(Func_02001cac, 0x17fa);
    Func_02001cc6(18, 0, 20);
    Func_02001d0a();
    record = Value1(Func_02001c30, 0);
    if ((*(volatile s32 *)((s32)record + 8) >> 20) == 26) {
        record = Value1(Func_02001c3e, 0);
        if ((*(volatile s32 *)((s32)record + 16) >> 20) > 13) {
            v7 = 1;
        }
    }
    if (v7 != 0) {
        Call3(Func_02001c5e, 0, 0xcccc, 0x6666);
        Call3(Func_02001d12, 0, 0xc000, 10);
        *(u8 *)(Func_02001c68(0) + 90) &= 254;
        Func_02001cda(0, 2);
        Func_02001ccc(0, 0, 16);
        Func_02001cda_a(0);
        Func_02001cf2(0, 1);
    }
    Call3(Func_02001ca4, 18, 0xcccc, 0x6666);
    record = Value1(Func_02001ca2, 18);
    if ((*(volatile s32 *)((s32)record + 16) >> 20) != 14) {
        record = Func_02001cb0(18);
        Func_02001cf4(18, *(s16 *)((s32)record + 10), 232);
    }
    Call3(Func_02001d00, 18, 0x118, 232);
    Call1(Func_02001ca8, 0x200);
    {
        u8 *record = Func_02001cd6(0);
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | 1);
    }
    Func_02001cdc();
    L_02000916:;
}

void FieldScene_RunScene3a0_02000968(void)
{
    u32 i;
    s32 record;
    s32 v5;
    u8 *p5;

    Func_02001d2e();
    *(u8 *)(Func_02001d44(20) + 35) &= 253;
    v5 = 0;
    *(u8 *)(Func_02001d54(20) + 85) = v5;
    record = Value1(Func_02001d60, 20);
    p5 = *(volatile s32 *)(record + 8);
    record = Value1(Func_02001d68, 20);
    Call6(Func_02001d2e_a, 3, 17, 1, 1, ((s32)p5 >> 20), (*(volatile s32 *)(record + 16) >> 20));
    Call2(Func_02001cf8, 0x2008325, 0xc80);
    Call1_02000968(Func_02001d66, 0x201);
    Func_02001e4e(20, 2);
    Func_02001d92();
}

void FieldScene_RunScene3a0_02000de8(s32 a0)
{
    u32 i;
    s32 record;

    *(u8 *)(Func_020021c0(0) + 85) = 0;
    Call3_02000de8(Func_020021dc, 0, 0x8000, 0x4000);
    if (a0 == 6) {
        Func_02002240(0, 2);
        Call3_02000de8(Func_02002234, 0, 0, -16);
    } else {
        Call3_02000de8(Func_0200223a, 0, 2, -16);
    }
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c8)) = 16;
    Func_020022d6(a0);
}

void FieldScene_RunScene3a0_02001060(void)
{
    u32 i;
    s32 record;

    Func_02002424();
    Call2((void (*)())Func_0200244c, 18, 1);
    record = Func_02002442(18);
    *(volatile s32 *)(record + 108) = 0;
    record = Func_0200244c_a(18);
    *(volatile s32 *)(record + 56) = -0x80000000;
    record = Value1(Func_02002458, 18);
    *(volatile s32 *)(record + 64) = -0x80000000;
    record = Value1(Func_02002460, 18);
    *(volatile s32 *)(record + 36) = 0;
    record = Value1(Func_02002468, 18);
    *(volatile s32 *)(record + 44) = 0;
    record = Value1(Func_02002470, 18);
    *(volatile s32 *)(record + 48) = 0;
    record = Func_02002478(18);
    *(volatile s32 *)(record + 52) = 0;
    Call3(Func_02002544, 18, 0x103, 0);
    Func_020024f4(18, 2);
    Func_0200247a(60);
    Call3(Func_020024a8, 18, 0x18000, 0xc000);
    Call3(Func_020024b6, 0, 0x18000, 0xc000);
    Call3(Func_020024ea, 18, 0x118, 232);
    Call3(Func_020024fe, 0, 0x128, 232);
    Func_0200251c(18);
    Call3(Func_02002588, 0, 0x8000, 20);
    Call3(Func_020025a4, 0, 0x102, 60);
    Call2(Func_020024fc, 18, 0x20095b0);
    record = Func_020024f2(18);
    *(volatile s32 *)(record + 108) = 0x2008501;
    Call0(Func_020024f2_a);
}
