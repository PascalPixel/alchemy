#include "types.h"

#define Lifted_02000bd0 Func_02000bd0

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
            Func_02002482(0x301);
        }
    }
}
