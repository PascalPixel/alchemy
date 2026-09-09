#include "types.h"

#define FieldScene_RunPrimarySequence Func_02000bd0
#define FieldScene_RunScene389_0200115c Func_0200115c
#define FieldScene_RunScene389_0200121c Func_0200121c
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern s16 Data_02000240[];
extern u8 Data_020098ec[];

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
s32 Func_02002142_a();
u8 *Func_020021e4_a();
s32 Func_020021ee_a();
void Func_02002204_a();
s32 Func_02002482_a();
void Func_0200251a();
void Func_020025a6();
void Func_020025da();
s32 Func_020025f8();
void Func_02002618();
void Func_0200262a();
void Func_0200262e();
void Func_02002634();
void Func_0200263c();
u8 *Func_02002646();
void Func_02002648();
void Func_02002652();
void Func_02002652_a();
void Func_02002654();
void Func_02002656();
void Func_0200265e();
void Func_02002664();
void Func_02002678();
void Func_02002046();
void Func_0200265e_a();
s32 Func_0200266a();
void Func_02002672();
u8 *Func_0200268a();
void Func_0200269c();
s32 Func_020026aa();
s32 Func_020026d2();
void Func_020026e4();
void Func_02002706();
void Func_02002720();
s32 Func_02002730();
void Func_02002734();
s32 Func_0200273c();
u8 *Func_02002748();
void Func_0200274c();
void Func_0200277c();
s32 Func_0200279a();
s32 Func_0200279a_a();
void Func_020027c4();
void Func_020027c6();
void Func_02002824();
void Func_02002830();

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

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_0200115c(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_0200121c(void (*f)(), s32 a0)
{
    f(a0);
}

void FieldScene_RunPrimarySequence(void)
{
    volatile s32 *state = (volatile s32 *)0x03001e40;
    s32 flags;
    s32 a0;
    s32 a2;
    s32 rec4;
    s32 rec7a, reca, v1a, v2a, v3a, ta, ua, na;
    s32 rec7b, recb, v1b, v2b, v3b, tb, ub, nb;

    flags = *state & 7;
    if (flags == 0) {
        rec7a = Value1(Func_0200203e, 9);
        v1a = Value0(Func_02001fb4);
        a0 = *(volatile s32 *)(rec7a + 8);
        a0 = a0 + (s32)((((u32)(((v1a << 1) + v1a) << 2)) >> 16) << 16);
        rec4 = Func_0200205a(9);
        reca = (s32)Func_02002062(9);
        a2 = *(volatile s32 *)(reca + 16);
        a2 = a2 + 0x60000;
        v2a = Value0(Func_02001fde);
        ta = (s32)((u32)((v2a << 2) + v2a) >> 16);
        ua = (((ta << 1) + ta) << 2) + ta;
        na = ua << 6;
        na = na - ua;
        na = na << 3;
        na = na + ta;
        v3a = Func_02001ff8();
        na = -na;
        Call7(Func_020016ba, a0, *(volatile s32 *)(rec4 + 12), a2, 0, na,
              (s32)((u32)(v3a << 1) >> 16), flags);
        flags = *state & 15;
        if (flags == 0) {
            rec7b = Value1(Func_020020b4, 9);
            v1b = Value0(Func_0200202a);
            a0 = *(volatile s32 *)(rec7b + 8);
            a0 = a0 + (s32)((((u32)(((v1b << 1) + v1b) << 2)) >> 16) << 16);
            rec4 = Func_020020d0(9);
            recb = (s32)Func_020020d8(9);
            a2 = *(volatile s32 *)(recb + 16);
            a2 = a2 + 0x60000;
            v2b = Value0(Func_02002054);
            tb = (s32)((u32)((v2b << 2) + v2b) >> 16);
            ub = (((tb << 1) + tb) << 2) + tb;
            nb = ub << 6;
            nb = nb - ub;
            nb = nb << 3;
            nb = nb + tb;
            v3b = Func_0200206e();
            nb = -nb;
            Call7(Func_02001730, a0, *(volatile s32 *)(rec4 + 12), a2, 0, nb,
                  (s32)((u32)(v3b << 1) >> 16), flags);
        }
    }
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
    ((void (*)())Func_02002142_a)();
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
        r5 = (s32)Func_020021e4_a(9);
        *(volatile s32 *)(r5 + 72) = 0x9999;
        Func_020021d6(3);
        Call3(Func_02002204_a, 9, 0x28000, 0x4000);
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
        v3 = Func_020021ee_a();
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

void Func_02000ecc(void)
{
    s32 a;
    s32 b;
    s32 x0, x1, x2;
    s32 c2;
    s32 q1, q2, q3, e1, e2;
    s32 g1a, g1b, g1c, g2a, g2b, g2c, g3a, g3b, g3c, g4a, g4b, g4c, g5a, g5b, g5c, g6a, g6b, g6c;

    q1 = Value1(Func_02002330, 10);
    a = *(volatile s32 *)(q1 + 8) / 0x100000;
    q2 = Value1(Func_02002342, 10);
    b = *(volatile s32 *)(q2 + 16) / 0x100000;
    if (a == 38) {
        if (b == 14) {
            q3 = (s32)Func_02002360(10);
            *(volatile s32 *)(q3 + 12) = -0x20000;
            e1 = Value1(Func_0200236a, 10);
            e2 = Value1(Func_02002372, 10);
            *(volatile s32 *)(e1 + 60) = *(volatile s32 *)(e2 + 12);
            Func_0200243c(188);
            g1a = Value1(Func_02002382, 10);
            g1b = Value1(Func_0200238a, 10);
            g1c = (s32)Func_02002392(10);
            Func_020019c8(*(s32 *)(g1a + 8), *(s32 *)(g1b + 12),
                  *(s32 *)(g1c + 16), 0x8000, 0, 0, 1);
            g2a = Value1(Func_020023b6, 10);
            g2b = Value1(Func_020023be, 10);
            g2c = (s32)Func_020023c6(10);
            Func_020019fa(*(s32 *)(g2a + 8), *(s32 *)(g2b + 12),
                  *(s32 *)(g2c + 16), 0x6666, 0x6666, 0, 1);
            g3a = Value1(Func_020023e8, 10);
            g3b = Value1(Func_020023f0, 10);
            g3c = (s32)Func_020023f8(10);
            x0 = *(s32 *)(g3a + 8);
            x1 = *(s32 *)(g3b + 12);
            x2 = *(s32 *)(g3c + 16);
            c2 = -0x6666;
            Func_02001a2e(x0, x1, x2, c2, 0x6666, 0, 1);
            g4a = Value1(Func_0200241c, 10);
            g4b = Value1(Func_02002424, 10);
            g4c = (s32)Func_0200242c(10);
            Func_02001a5c(*(s32 *)(g4a + 8), *(s32 *)(g4b + 12),
                  *(s32 *)(g4c + 16), -0x8000, 0, 0, 1);
            g5a = Value1(Func_0200244a, 10);
            g5b = Value1(Func_02002452, 10);
            g5c = (s32)Func_0200245a(10);
            Func_02001a8c(*(s32 *)(g5a + 8), *(s32 *)(g5b + 12),
                  *(s32 *)(g5c + 16), 0x6666, c2, 0, 1);
            g6a = Value1(Func_0200247a, 10);
            g6b = Value1(Func_02002482, 10);
            g6c = (s32)Func_0200248a(10);
            Func_02001abc(*(s32 *)(g6a + 8), *(s32 *)(g6b + 12),
                  *(s32 *)(g6c + 16), c2, c2, 0, 1);
            Func_02002482_a(0x301);
        }
    }
}

void FieldScene_RunScene389_0200115c(void)
{
    u32 i;
    u8 *record;

    Call1_0200115c(Func_0200251a, 0x20090c9);
    Func_020025a6();
    Call3(Func_02002652, 13, 0x100, 30);
    Func_0200262a(13, 2);
    Call3(Func_0200265e, 0, 0xa000, 0);
    Call1_0200115c(Func_02002654, 0x132f);
    Func_02002664(13, 0);
    Func_02002634(13, 3);
    Func_020025da(30);
    *(u8 *)(Func_020025f8(10) + 35) &= 253;
    Call3(Func_02002618, 13, 0x20000, 0x10000);
    Call3(Func_0200263c, 13, 0x258, 216);
    Call3(Func_02002648, 13, 0x258, 248);
    Call3(Func_02002656, 13, 0x238, 0x128);
    Func_02002678(13, 0, 0);
    {
        u8 *record = Func_02002646(10);
        u8 value = *(volatile u8 *)&record[35];

        record[35] = (u8)(value | 2);
    }
    Call1_0200115c(Func_0200262e, 0x869);
    Func_02002652_a();
}

s32 FieldScene_RunScene389_0200121c(void)
{
    u8 *record;
    u8 *work;

    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0x204;
    *(volatile s32 *)(((s32)work + 0x1c8)) = 24;
    {
        u8 *record = Func_0200268a(9);
        u8 value = *(volatile u8 *)&record[89];

        record[89] = (u8)(value | 16);
    }
    if (Value1(Func_0200266a, 0x302) != 0) {
        Call3(Func_020026e4, 8, 0x1580000, 0x680000);
        Call6(Func_0200265e_a, 24, 40, 6, 3, 18, 6);
    } else {
        Call6(Func_02002672, 18, 40, 6, 3, 18, 6);
    }
    if (Value1(Func_020026aa, 0x300) != 0) {
        Func_02002720(9, 0, 0);
        Call6(Func_0200269c, 21, 45, 4, 2, 21, 11);
    }
    if (Value1(Func_020026d2, 0x301) != 0) {
        Call3(Func_0200274c, 10, 0x2680000, 0xe80000);
        if ((u32)(((u16)Data_02000240[225] - 2) << 16) > 0x10000) {
            goto L_0200131c;
        }
        *(u8 *)(Func_02002730(10) + 34) = 2;
        record = Value1(Func_0200273c, 10);
        *(volatile s32 *)((s32)record + 12) = (*(volatile s32 *)((s32)record + 12) - 1);
        {
            u8 mask = 2;
            u8 *record = Func_02002748(10);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | mask);
        }
        Call6(Func_02002706, 36, 48, 5, 1, 36, 14);
    } else {
    }
    L_0200131c:;
    if (Data_02000240[225] == 99) {
        Func_02002824();
        Func_02002830();
        Call3(Func_020027c6, 9, 0x1800000, 0xc00000);
        Func_0200277c(60);
        *(u8 *)(Func_0200279a(9) + 34) = 2;
        Call3(Func_020027c4, 9, 0x198, 192);
        Call1_0200121c((void (*)())Func_0200279a_a, 60);
        Func_02002046();
    }
    if (Data_02000240[282] != 0) {
        {
            s32 zero = 0;
            *(volatile s32 *)Data_020098ec = zero;
        }
        Call2(Func_02002734, 0x20090c9, 0xc80);
    }
    return 0;
}
