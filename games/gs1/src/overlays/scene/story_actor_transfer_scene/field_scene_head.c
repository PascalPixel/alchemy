#include "types.h"

#define FieldScene_RunStep7D3B1E Func_02000580
#define FieldScene_RunStep74 Func_020006dc
#define FieldScene_RunScene371_0200155c Func_0200155c
#define FieldScene_RunScene371_02001680 Func_02001680
#define FieldScene_RunScene371_020017a4 Func_020017a4
#define FieldScene_RunScene371_020017fc Func_020017fc
#define FieldScene_RunScene371_02001888 Func_02001888
#define FieldScene_RunScene371_02001938 Func_02001938
#define FieldScene_RunScene371_020019e8 Func_020019e8
#define FieldScene_RunScene371_02001a98 Func_02001a98
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];

void Func_02000a7a(s32, s32, s32);
void Func_020049bc(void);
void Func_02004af2(s32);
void Func_020056f6();
void Func_0200570a();
void Func_0200571e();
void Func_02005732();
void Func_02005746();
void Func_0200575c();
void Func_02005772();
void Func_02005786();
void Func_0200579a();
void Func_020057ae();
void Func_020057c2();
void Func_020057d8();
void Func_02005960();
void Func_02005974();
void Func_02005988();
void Func_0200599c();
void Func_020059b0();
void Func_020059c6();
void Func_020059dc();
void Func_020059f0();
void Func_02005a04();
void Func_02005a18();
void Func_02005a2c();
void Func_02005a42();
void Func_0200581a();
void Func_0200582e();
void Func_02005842();
void Func_02005856();
void Func_0200586a();
void Func_02005880();
void Func_02005896();
void Func_020058aa();
void Func_020058be();
void Func_020058d2();
void Func_020058e6();
void Func_020058fc();
void Func_02005a84();
void Func_02005a98();
void Func_02005aac();
void Func_02005ac0();
void Func_02005ad4();
void Func_02005aea();
void Func_02005b00();
void Func_02005b14();
void Func_02005b28();
void Func_02005b3c();
void Func_02005b50();
void Func_02005b66();
void Func_020059e0();
s32 Func_02005a5e();
void Func_02005a80();
s32 Func_02005aa6();
s32 Func_02005aae();
void Func_02005c80();
void Func_020059a4();
void Func_02005ae4();
void Func_02005af4();
s32 Func_02005afe();
void Func_02005b4e();
void Func_02005b54();
void Func_02005b86();
void Func_02005b96();
void Func_02005c0e();
void Func_02005c1e();
void Func_02005c72();
void Func_02005c7a();
void Func_02005c9a();
void Func_02005ca6();
void Func_02005a30();
void Func_02005a54();
void Func_02005a7e();
void Func_02005b70();
s32 Func_02005b8a();
void Func_02005ba4();
void Func_02005bf2();
void Func_02005bfe();
void Func_02005c08();
void Func_02005c12();
void Func_02005c1e_a();
void Func_02005c9a_a();
void Func_02005cbc();
void Func_02005d18();
void Func_02005d22();
void Func_02005d4a();
void Func_02005d56();
void Func_02005ae0();
void Func_02005b04();
void Func_02005b2e();
void Func_02005c20();
s32 Func_02005c3a();
void Func_02005c54();
void Func_02005ca2();
void Func_02005cae();
void Func_02005cb8();
void Func_02005cc2();
void Func_02005cce();
void Func_02005d4a_a();
void Func_02005d6c();
void Func_02005dc8();
void Func_02005dd2();
void Func_02005dfa();
void Func_02005e06();
void Func_02005b90();
void Func_02005bb4();
void Func_02005bde();
void Func_02005cd0();
s32 Func_02005cea();
void Func_02005d04();
void Func_02005d52();
void Func_02005d5e();
void Func_02005d68();
void Func_02005d72();
void Func_02005d7e();
void Func_02005dfa_a();
void Func_02005e1c();
void Func_02005e78();
void Func_02005e82();
void Func_02005eaa();
void Func_02005eb6();
s32 Func_02004f1c();
void Func_02005c40();
void Func_02005c64();
void Func_02005ca0();
void Func_02005d80();
s32 Func_02005d9a();
void Func_02005dc6();
void Func_02005e02();
void Func_02005e20();
void Func_02005e20_a();
void Func_02005e22();
void Func_02005e2a();
void Func_02005e2e();
void Func_02005eaa_a();
void Func_02005ecc();
void Func_02005f28();
void Func_02005f44();
void Func_02005f6c();
void Func_02005f78();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4_020017fc(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call4_02001888(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call4_02001938(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call4_020019e8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call1_02001a98(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call4_02001a98(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void FieldScene_RunStep7D3B1E(void)
{
    Func_02000a7a(0x7D, 0x3B, 0x1E);
}

void FieldScene_RunStep74(void)
{
    Func_020049bc();
    Func_02004af2(74);
}

void FieldScene_RunScene371_0200155c(void)
{
    u32 i;
    s32 record;

    Call4(Func_02005960, 0x160c0000, -1, 0x6f80000, 1);
    Func_020056f6(4);
    Call4(Func_02005974, 0x16040000, -1, 0x6fc0000, 1);
    Func_0200570a(4);
    Call4(Func_02005988, 0x160c0000, -1, 0x6f40000, 1);
    Func_0200571e(4);
    Call4(Func_0200599c, 0x160c0000, -1, 0x6fc0000, 1);
    Func_02005732(4);
    Call4(Func_020059b0, 0x16040000, -1, 0x6f40000, 1);
    Func_02005746(4);
    Call4(Func_020059c6, 0x16080000, -1, 0x6f80000, 1);
    Func_0200575c(4);
    Call4(Func_020059dc, 0x160a0000, -1, 0x6f80000, 1);
    Func_02005772(4);
    Call4(Func_020059f0, 0x16060000, -1, 0x6fa0000, 1);
    Func_02005786(4);
    Call4(Func_02005a04, 0x160a0000, -1, 0x6f60000, 1);
    Func_0200579a(4);
    Call4(Func_02005a18, 0x160a0000, -1, 0x6fa0000, 1);
    Func_020057ae(4);
    Call4(Func_02005a2c, 0x16060000, -1, 0x6f60000, 1);
    Func_020057c2(4);
    Call4(Func_02005a42, 0x16080000, -1, 0x6f80000, 1);
    Func_020057d8(4);
}

void FieldScene_RunScene371_02001680(void)
{
    u32 i;
    s32 record;

    Call4(Func_02005a84, 0x15ec0000, -1, 0x6c80000, 1);
    Func_0200581a(4);
    Call4(Func_02005a98, 0x15e40000, -1, 0x6cc0000, 1);
    Func_0200582e(4);
    Call4(Func_02005aac, 0x15ec0000, -1, 0x6c40000, 1);
    Func_02005842(4);
    Call4(Func_02005ac0, 0x15ec0000, -1, 0x6cc0000, 1);
    Func_02005856(4);
    Call4(Func_02005ad4, 0x15e40000, -1, 0x6c40000, 1);
    Func_0200586a(4);
    Call4(Func_02005aea, 0x15e80000, -1, 0x6c80000, 1);
    Func_02005880(4);
    Call4(Func_02005b00, 0x15ea0000, -1, 0x6c80000, 1);
    Func_02005896(4);
    Call4(Func_02005b14, 0x15e60000, -1, 0x6ca0000, 1);
    Func_020058aa(4);
    Call4(Func_02005b28, 0x15ea0000, -1, 0x6c60000, 1);
    Func_020058be(4);
    Call4(Func_02005b3c, 0x15ea0000, -1, 0x6ca0000, 1);
    Func_020058d2(4);
    Call4(Func_02005b50, 0x15e60000, -1, 0x6c60000, 1);
    Func_020058e6(4);
    Call4(Func_02005b66, 0x15e80000, -1, 0x6c80000, 1);
    Func_020058fc(4);
}

void FieldScene_RunScene371_020017a4(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_02005aa6, 15);
    record = Value1(Func_02005aae, 14);
    *(s32 *)(rec7 + 8) = *(s32 *)(record + 8);
    *(s32 *)(rec7 + 16) = *(s32 *)(record + 16);
    if (*(s32 *)(rec7 + 12) < 0xa0000) {
        *(s32 *)(rec7 + 12) = 0xa0000;
        if (Value1(Func_02005a5e, 0x200) == 0) {
            Func_02005c80(145);
            Func_020059e0(rec7, 3);
            Call1(Func_02005a80, 0x200);
            {
                u16 *target = (u16 *)(rec7 + 100);
                s32 shown = 1;

                *target = shown;
            }
        }
    }
}

void FieldScene_RunScene371_020017fc(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_02005afe, 8);
    Func_02005ae4();
    Call4_020017fc(Func_02005c0e, -1, -1, -1, 0);
    Func_020059a4(1);
    Func_02005b86(0, 0, 0);
    *(s32 *)(rec7 + 28) = 0x14000;
    *(s32 *)(rec7 + 24) = 0x14000;
    Func_02005c1e(8, 1);
    Func_02005c7a();
    Call3(Func_02005b54, 8, 0x6666, 0x3333);
    Call3(Func_02005b96, 8, 0x14a8, 0x918);
    Func_02005c9a();
    Func_02005ca6();
    Call1(Func_02005af4, 0x927);
    Func_02005c72(102);
    Func_02005b4e();
}

void FieldScene_RunScene371_02001888(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_02005b8a, 8);
    Func_02005b70();
    Call4_02001888(Func_02005c9a_a, -1, -1, -1, 0);
    Func_02005a30(1);
    Func_02005c12(0, 0, 0);
    Call3(Func_02005c1e_a, 8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Func_02005a54(1);
    Func_02005cbc(8, 1);
    Func_02005d18();
    Call3(Func_02005bf2, 8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Func_02005c08(8, 0x200d158);
    do {
        Func_02005a7e(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Func_02005d4a();
    Func_02005d56();
    Call1(Func_02005ba4, 0x927);
    Func_02005d22(103);
    Func_02005bfe();
}

void FieldScene_RunScene371_02001938(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_02005c3a, 8);
    Func_02005c20();
    Call4_02001938(Func_02005d4a_a, -1, -1, -1, 0);
    Func_02005ae0(1);
    Func_02005cc2(0, 0, 0);
    Call3(Func_02005cce, 8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Func_02005b04(1);
    Func_02005d6c(8, 1);
    Func_02005dc8();
    Call3(Func_02005ca2, 8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Func_02005cb8(8, 0x200d158);
    do {
        Func_02005b2e(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Func_02005dfa();
    Func_02005e06();
    Call1(Func_02005c54, 0x927);
    Func_02005dd2(104);
    Func_02005cae();
}

void FieldScene_RunScene371_020019e8(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_02005cea, 8);
    Func_02005cd0();
    Call4_020019e8(Func_02005dfa_a, -1, -1, -1, 0);
    Func_02005b90(1);
    Func_02005d72(0, 0, 0);
    Call3(Func_02005d7e, 8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Func_02005bb4(1);
    Func_02005e1c(8, 1);
    Func_02005e78();
    Call3(Func_02005d52, 8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Func_02005d68(8, 0x200d158);
    do {
        Func_02005bde(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Func_02005eaa();
    Func_02005eb6();
    Call1(Func_02005d04, 0x927);
    Func_02005e82(105);
    Func_02005d5e();
}

void FieldScene_RunScene371_02001a98(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_02005d9a, 8);
    Func_02005d80();
    Call4_02001a98(Func_02005eaa_a, -1, -1, -1, 0);
    Func_02005c40(1);
    Func_02005e22(0, 0, 0);
    Call3(Func_02005e2e, 8, 0x1f080000, 0xc80000);
    *(s32 *)(rec7 + 24) = 0x14000;
    *(s32 *)(rec7 + 28) = 0x14000;
    Func_02005c64(1);
    Func_02005ecc(8, 1);
    Func_02005f28();
    Call3(Func_02005e02, 8, 0x9999, 0x4ccc);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    if (Value0(Func_02004f1c) == 11) {
        Func_02005e20(8, 0x200d1b8);
    } else {
        Func_02005e2a(8, 0x200d158);
    }
    do {
        Func_02005ca0(1);
    } while (*(s16 *)(rec7 + 100) == 0);
    Func_02005f6c();
    Func_02005f78();
    Call1_02001a98(Func_02005dc6, 0x927);
    Func_02005f44(106);
    Func_02005e20_a();
}
