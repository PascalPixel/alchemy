#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_02000d4c
#define FieldScene_RunScene3c5SequenceA Func_02000eac
#define FieldScene_RunStep11 Func_02001184
#define FieldScene_RunStep12WithPosition Func_0200119c
#define FieldScene_RunStep13WithTwoPositions Func_020011c4
#define FieldScene_RunStep15 Func_020011fc
#define FieldScene_RunStepWithValue2693 Func_02001214
#define FieldScene_RunScene3c5_020024d0 Func_020024d0
#define FieldScene_RunScene3c5_02002548 Func_02002548
#define FieldScene_RunFlag985DialogueBranch Func_020025c8
void Func_0200190c();
void Func_02003af4();
void Func_02003b1e();
s32 Func_02003b7c();
s32 Func_02003b80();
void Func_02003b90();
s32 Func_02003bca();
s32 Func_02003bd6();
void Func_02003bde();
s32 Func_02003be2();
s32 Func_02003bec();
void Func_02003bf6();
s32 Func_02003c0a();
s32 Func_02003c1c();
void Func_02003c4e();
void Func_02003c5c();
void Func_02003c68();
void Func_02003c8a();
s32 Func_02003cc0();
void Func_02003cc6();
s32 Func_02003ccc();
void Func_02003cce();
void Func_02003cf4();
void Func_02003d08();
void Func_02003d3e();
void Func_02003d44();
void Func_02003d72();
void Func_02003dbe();
void Func_02001c0a();
void Func_02003c4e_a();
void Func_02003c78();
void Func_02003cea();
void Func_02003cf6();
s32 Func_02003d24();
s32 Func_02003d30();
s32 Func_02003d3c();
void Func_02003d44_a();
s32 Func_02003d46();
s32 Func_02003d50();
s32 Func_02003d70();
s32 Func_02003d7c();
void Func_02003d86();
void Func_02003d8c();
void Func_02003de4();
void Func_02003e0a();
void Func_02003e5a();
void Func_02003e6e();
void Func_02003e78();
void Func_02003eaa();
void Func_02003fc0();
void Func_020022e4();
void Func_020040c2();
void Func_02003fd6();
void Func_02003fd8();
void Func_02001ba4();
void Func_0200230c();
void Func_020040ea();
void Func_02003ffe();
void Func_02004000();
s32 Func_02001bcc();
void Func_02001bdc();
void Func_02002344();
void Func_02004122();
void Func_02004036();
void Func_02004038();
void Func_0200235c();
void Func_0200413a();
void Func_0200404e();
void Func_02004050();
void Func_020040b0();
void Func_02004038_a();
void Func_0200406c();
void Func_02004046();
s32 Func_020052f0();
void Func_020052fc();
void Func_02005302();
void Func_02005324();
s32 Func_02005346();
void Func_02005358();
void Func_02005364();
void Func_02005368();
void Func_0200538a();
void Func_02005390();
void Func_02005478();
s32 Func_02005368_a();
void Func_02005374();
void Func_02005382();
void Func_0200539c();
s32 Func_020053be();
void Func_020053d0();
void Func_020053d8();
void Func_020053d8_a();
void Func_020053dc();
void Func_020053e0();
void Func_02005402();
void Func_02005408();
void Func_020054f0();
void Func_0200540a();
s32 Func_020053fc();
void Func_02005400();
void Func_02005400_a();
void Func_02005586();
void Func_020053ea();
void Func_02005440();
void Func_02005456();
void Func_02004afa();
void Func_0200550a();
void Func_02005524();
void Func_02005480();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Contiguous unnamed leaf-owner run for resource_3c5. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call4_02000eac(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value1_020024d0(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value1_02002548(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

void FieldScene_RunSupplementalSequenceOne(s32 a0)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 rec4;
    s32 zero;
    s32 t;
    s32 nv;
    u8 *p8;
    u8 *p6;
    u8 slot28[40];
    u8 slot16[12];

    Func_02003b90(a0);
    Call4(Func_02003c8a, -1, -1, -1, 0);
    Func_02003b1e();
    Func_02003af4(1);
    *(volatile s32 *)(Func_02003bca(0) + 12) = 0x820000;
    *(volatile s32 *)(Func_02003bd6(0) + 72) = 0x8000;
    zero = 0;
    *(volatile s32 *)(Func_02003be2(0) + 68) = zero;
    *(u8 *)(Func_02003bec(0) + 85) = zero;
    Func_02003cf4();
    Func_02003d08();
    Func_02003bde(30);
    Func_02003d44(204);
    *(u8 *)(Func_02003c0a(0) + 85) = 3;
    Func_02003bf6(24);
    rec4 = Value1(Func_02003c1c, 0);
    p8 = slot28;
    *(s32 *)(p8 + 4) = 7;
    *(s32 *)(p8 + 36) = 0x2008cf1;
    *(s32 *)(p8 + 8) = 0xcccc;
    *(s32 *)(p8 + 12) = 0xcccc;
    i = 0;
    p6 = slot16;
    for (; i < 17; i++) {
        *(s32 *)(p6) = Func_02003b7c(i << 12);
        *(s32 *)(p6 + 4) = 0;
        t = Func_02003b80(i << 12);
        nv = *(s32 *)(p6);
        nv = nv + nv / 2;
        *(s32 *)(p6 + 8) = t;
        *(s32 *)(p6) = nv;
        Func_0200190c(*(s32 *)(rec4 + 8), *(s32 *)(rec4 + 12),
              *(s32 *)(rec4 + 16), nv,
              *(s32 *)(p6 + 4), t, 0x1090001, (s32)p8);
    }
    Func_02003dbe(188);
    Call2(Func_02003d3e, 0, 0x101);
    Func_02003cce(0, 22);
    Call3(Func_02003c4e, 0x50000, 0x50000, 0x10000);
    Call3(Func_02003c5c, -1, -1, 0xe666);
    Func_02003c68();
    Call2(Func_02003d72, 0, 0x100);
    *(volatile s32 *)(Func_02003cc0(0) + 72) = 0x10000;
    *(volatile s32 *)(Func_02003ccc(0) + 68) = 0x4000;
    Func_02003cc6();
}

void FieldScene_RunScene3c5SequenceA(s32 a0)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;
    s32 v5;

    Func_02003cea();
    Call4_02000eac(Func_02003de4, -1, -1, -1, 0);
    Func_02003c78();
    Func_02003c4e_a(1);
    record = Func_02003d24(0);
    *(volatile s32 *)(record + 12) = 0x820000;
    record = Func_02003d30(0);
    *(volatile s32 *)(record + 72) = 0x4000;
    v5 = 0;
    record = Func_02003d3c(0);
    *(volatile s32 *)(record + 68) = v5;
    *(u8 *)(Func_02003d46(0) + 85) = v5;
    record = Func_02003d50(0);
    Func_02003cf6(record, 0);
    Func_02003e5a();
    Func_02003e6e();
    Func_02003d44_a(10);
    Func_02003eaa(204);
    *(u8 *)(Func_02003d70(0) + 85) = 3;
    record = Func_02003d7c(0);
    *(volatile s32 *)(record + 40) = -0x50000;
    Func_02003d86(0);
    Func_02001c0a();
    Func_02003e0a(0, 15);
    Func_02003e78(a0);
    Func_02003d8c();
}

void FieldScene_RunStep11(void)
{
    Func_02003fc0();
    Func_020022e4();
    Func_020040c2(11);
    Func_02003fd6();
}

void FieldScene_RunStep12WithPosition(void)
{
    Func_02003fd8();
    Func_02001ba4(0x1d00000, 0, 0x1220000, 223);
    Func_0200230c();
    Func_020040ea(12);
    Func_02003ffe();
}

void FieldScene_RunStep13WithTwoPositions(void)
{
    Func_02004000();
    Func_02001bcc(0x8f0000, 0, 0x1220000, 223);
    Func_02001bdc(0x790000, 0, 0x11e0000, 253);
    Func_02002344();
    Func_02004122(13);
    Func_02004036();
}

void FieldScene_RunStep15(void)
{
    Func_02004038();
    Func_0200235c();
    Func_0200413a(15);
    Func_0200404e();
}

void FieldScene_RunStepWithValue2693(void)
{
    Func_02004050();
    Func_020040b0(0, 1);
    Func_02004038_a(0x2693, 1);
    Func_0200406c();
}

void FieldScene_RunScene3c5_020024d0(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    if (Value1_020024d0(Func_020052f0, 0x985) == 0) {
        Call1(Func_02005302, 0x985);
        Func_02005478(157);
        Func_02005324();
        Call3(Func_02005358, 8, 0x118, 240);
        Call3(Func_02005364, 9, 0x148, 240);
        Func_0200538a(8);
        Func_02005390(9);
        Call6(Func_020052fc, 81, 14, 4, 1, 17, 14);
        Func_02005368();
        if (Value1_020024d0(Func_02005346, 0x989) == 0) {
            Func_02004046();
        }
    }
}

void FieldScene_RunScene3c5_02002548(void)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    if (Value1_02002548(Func_02005368_a, 0x985) != 0) {
        Call1(Func_02005382, 0x985);
        Func_020054f0(157);
        Func_0200539c();
        Call3(Func_020053d0, 8, 0x128, 240);
        Call3(Func_020053dc, 9, 0x138, 240);
        Func_02005402(8);
        Func_02005408(9);
        Call6(Func_02005374, 0, 14, 4, 1, 17, 14);
        Func_020053e0();
        if (Value1_02002548(Func_020053be, 0x301) != 0) {
            Call1(Func_020053d8, 0x301);
        } else {
            Call1(Func_020053d8_a, 0x301);
        }
    }
}

void FieldScene_RunFlag985DialogueBranch(void)
{
    extern u8 *Data_03001ebc;

    u8 *base = Data_03001ebc;
    s16 *h;

    Func_0200540a();
    h = (s16 *)(base + 0xcb8);
    if (h[0] != 0) {
        if (Func_020053fc(0x985) == 0) {
            s32 k5 = 17, k6 = 78;

            Func_02005400(0x1528, 1);
            Func_02005586(155);
            Func_020053ea(35, 78, 1, 2, k5, k6);
            Func_02005440(10);
            Func_02005400_a(34, 78, 1, 2, k5, k6);
            Func_02005456(10);
            Func_02004afa();
        }
    } else {
        Func_0200550a(0x2756);
        Func_02005524(-1, 0);
    }
    Func_02005480();
}
