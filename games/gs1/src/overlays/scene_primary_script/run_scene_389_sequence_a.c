#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020016ba();
void Func_02001730();
void Func_020018b0();
void Func_020019c8();
void Func_020019fa();
void Func_02001a2e();
void Func_02001a5c();
void Func_02001a8c();
void Func_02001abc();
s32 Func_02001fb4();
s32 Func_02001fde();
s32 Func_02001ff8();
s32 Func_0200202a();
s32 Func_0200203e();
s32 Func_02002054();
s32 Func_0200205a();
u8 *Func_02002062();
s32 Func_0200206e();
s32 Func_020020b4();
s32 Func_020020d0();
u8 *Func_020020d8();
void Func_02002108();
void Func_02002124();
void Func_02002140();
s32 Func_02002142();
u8 *Func_0200215e();
u8 *Func_0200216e();
u8 *Func_02002182();
void Func_02002184();
void Func_0200218a();
s32 Func_020021aa();
void Func_020021ac();
void Func_020021cc();
void Func_020021ce();
s32 Func_020021d4();
void Func_020021d6();
u8 *Func_020021da();
void Func_020021e0();
u8 *Func_020021e4();
s32 Func_020021ee();
void Func_020021f6();
void Func_02002204();
void Func_02002210();
u8 *Func_02002216();
void Func_02002220();
s32 Func_02002234();
s32 Func_02002250();
u8 *Func_02002258();
void Func_0200226e();
void Func_02002274();
void Func_02002282();
void Func_02002286();
void Func_0200228e();
void Func_02002296();
void Func_020022ba();
void Func_020022c4();
void Func_020022ee();
void Func_020022f2();
void Func_02002308();
void Func_02002312();
s32 Func_02002330();
s32 Func_02002342();
u8 *Func_02002360();
void Func_02002364();
s32 Func_0200236a();
s32 Func_02002372();
s32 Func_02002382();
s32 Func_0200238a();
u8 *Func_02002392();
s32 Func_020023b6();
s32 Func_020023be();
s32 Func_020023c6();
s32 Func_020023e8();
s32 Func_020023f0();
s32 Func_020023f8();
s32 Func_0200241c();
s32 Func_02002424();
u8 *Func_0200242c();
void Func_0200243c();
s32 Func_0200244a();
s32 Func_02002452();
s32 Func_0200245a();
s32 Func_0200247a();
s32 Func_02002482();
s32 Func_0200248a();
void Func_02008bd1();

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call7(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{
    f(a0, a1, a2, a3, a4, a5, a6);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}


void Func_02000ce0(void)
{
    s32 kind;
    s32 zero;
    s32 base;
    s32 a0;
    s32 a2;
    s32 rec4;
    s32 r1, r2, r3, r4, r5, r6, r7;
    s32 rec7, v1, v2, v3, t, u, n;
    u8 *p0;

    r1 = Value1(Func_02002142, 9);
    kind = *(volatile s32 *)(r1 + 8) / 0x100000;
    ((void (*)())Func_02002142)();
    if (kind == 25) {
        p0 = Func_0200215e(11);
        zero = 0;
        p0[34] = 1;
        r2 = (s32)Func_0200216e(11);
        Func_02002124(r2, 0);
        Call2((void (*)())Func_020021e4, 11, 14);
        r3 = (s32)Func_02002182(11);
        Func_02002140(r3, 1);
        Call3(Func_020021ce, 11, 0x19e0000, 0xf00000);
        Func_02002184(10);
        base = (s32)Func_02008bd1;
        Func_02002108(base, 0xc80);
        Func_0200226e(141);
        Func_020021e0(9, 1, 0);
        Call1((void (*)())Func_020021ee, 9);
        Func_020021ac(10);
        Func_020021f6(9, 2, 0);
        Func_02002204(9);
        r4 = (s32)Func_020021da(9);
        *(volatile s32 *)(r4 + 68) = zero;
        r5 = (s32)Func_020021e4(9);
        *(volatile s32 *)(r5 + 72) = 0x9999;
        Func_020021d6(3);
        Call3(Func_02002204, 9, 0x28000, 0x4000);
        Call1(Func_020022c4, 0x120);
        Call3(Func_02002220, 9, 0x1a0, 200);
        r6 = (s32)Func_02002216(9);
        Func_020021cc(r6, 0);
        Func_0200218a(base);
        Func_02002210(12);
        Func_020022ee(189);
        rec7 = Value1(Func_02002234, 9);
        v1 = Value0(Func_020021aa);
        a0 = *(volatile s32 *)(rec7 + 8);
        a0 = a0 + (s32)((((u32)(((v1 << 1) + v1) << 2)) >> 16) << 16);
        rec4 = Func_02002250(9);
        r7 = (s32)Func_02002258(9);
        a2 = *(volatile s32 *)(r7 + 16);
        a2 = a2 + 0x60000;
        v2 = Value0(Func_020021d4);
        t = (s32)((u32)((v2 << 2) + v2) >> 16);
        u = (((t << 1) + t) << 2) + t;
        n = u << 6;
        n = n - u;
        n = n << 3;
        n = n + t;
        v3 = Func_020021ee();
        n = -n;
        Call7(Func_020018b0, a0, *(volatile s32 *)(rec4 + 12), a2, zero, n,
              (s32)((u32)(v3 << 1) >> 16), zero);
        Func_02002286(20);
        Func_02002364(154);
        Call3(Func_02002274, 0x50000, 0x50000, 0x10000);
        Call3(Func_02002282, -1, -1, 0xe666);
        Func_0200228e();
        Func_02002308(9, 0, 0);
        Call3(Func_02002312, 11, 0, 0);
        Call1(Func_020022ba, 0x300);
        Call6(Func_02002296, 21, 45, 4, 2, 21, 11);
    }
    Func_020022f2();
}

