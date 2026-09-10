#include "types.h"

#define FieldScene_RunScene3a7SequenceA Func_020003e0
#define FieldScene_RunGuardedStep9AAAfterSetup Func_0200048c
#define FieldScene_RunGuardedStep9AA Func_020004d0
#define FieldScene_RunSupplementalSequenceOne Func_02000754
#define FieldScene_RunScene3a7SequenceB Func_020008a8
#define FieldScene_RunOpeningAuxiliarySequence Func_020009c0
extern u8 Data_03001ebc[];
extern u8 Data_0200a224[];
extern u8 Data_0200a228[];
extern u8 Data_0200a22c[];
extern u8 Data_0200a230[];

s32 Func_02000722();
void Func_0200079a();
s32 Func_020014e6();
void Func_02001500();
void Func_0200154e();
void Func_02000600();
void Func_020008c6();
s32 Func_020008f6();
s32 Func_02001596();
s32 Func_020015a8();
void Func_020015c4();
void Func_02001602();
void Func_02000906();
s32 Func_02000936();
s32 Func_020015d6();
s32 Func_020015e8();
void Func_02001604();
void Func_02001642();
s32 Func_02001836();
void Func_02001882();
void Func_0200188c();
void Func_02001890();
s32 Func_02001898();
void Func_020018ba();
void Func_020018fe();
void Func_0200191a();
void Func_02001920();
void Func_02001926();
void Func_02001948();
void Func_0200195e();
void Func_0200199a();
void Func_020019b6();
double Func_02001bd2();
void Func_02001bde();
double Func_02001c1c();
double Func_02001c2a();
s32 Func_02001cd6();
void Func_02001084();
void Func_020019a4();
s32 Func_020019b0();
void Func_020019b4();
void Func_020019ba();
void Func_020019ca();
void Func_020019d8();
void Func_020019ee();
void Func_020019fa();
void Func_02001a10();
void Func_02001a18();
void Func_02001a2e();
void Func_02001a44();
void Func_02001a4e();
s32 Func_0200138a();
s32 Func_020013ba();
void Func_020015b0();
s32 Func_02001af6();
u8 *Func_02001b22();
s32 Func_02001b36();
s32 Func_02001b40();
s32 Func_02001b48();
s32 Func_02001b58();
void Func_02001b6a();
void Func_02001b8a_a();
void Func_02001b92();
void Func_02001bb8();
void Func_02001bc0();
void Func_02001bd0();
s32 Func_02001bd6();
s32 Func_02001bf6();
u8 *Func_02001c06();
u8 *Func_02001c1a();
void Func_02001c66();
void Func_02001c82();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

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
static __inline__ void Call1_02000754(void (*f)(), s32 a0)
{
    double Func_02001b8a();

    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    double Func_02001b8a();

    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    double Func_02001b8a();

    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    double Func_02001b8a();

    f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    double Func_02001b8a();

    return f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    double Func_02001b8a();

    f(a0, a1, a2, a3, a4, a5);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_020008a8(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_020009c0(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_02000aa0(void (*f)(), s32 a0)
{
    void Func_02001b8a();

    f(a0);
}

void FieldScene_RunScene3a7SequenceA(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_020014e6, 0x9a9) == 0) {
        if (Value0(Func_02000722)!= 0) {
            Call1(Func_02001500, 0x9a9);
            Func_0200154e(80);
            Func_0200079a();
        }
    }
}

void FieldScene_RunGuardedStep9AAAfterSetup(void)
{
    u32 i;
    s32 record;

    Func_02000600();
    if (Value1(Func_02001596, 0x9aa) == 0) {
        if (Value0(Func_020008f6)!= 0) {
            if (Value1(Func_020015a8, 0x207) == 0) {
                Func_02001602(80);
                Func_020008c6();
                Call1(Func_020015c4, 0x9aa);
            }
        }
    }
}

void FieldScene_RunGuardedStep9AA(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_020015d6, 0x9aa) == 0) {
        if (Value0(Func_02000936)!= 0) {
            if (Value1(Func_020015e8, 0x207) == 0) {
                Func_02001642(80);
                Func_02000906();
                Call1(Func_02001604, 0x9aa);
            }
        }
    }
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    double Func_02001b8a();

    s32 p10;
    s32 count;
    s32 rec7;
    s32 shown;
    double bias;
    double held;
    double scaled;
    s32 c5;
    s32 c6;
    s32 mode;

    p10 = (0x164 + *(volatile s32 *)0x03001e70);
    Func_0200188c();
    if ((*(volatile s32 *)0x03001e40 & 1) != 0) {
        *(volatile s32 *)(p10 + 24) = 1;
        *(volatile s32 *)(p10 + 28) = 1;
    } else {
        *(volatile s32 *)(p10 + 24) = -1;
        *(volatile s32 *)(p10 + 28) = -1;
    }
    Call3(Func_02001882, 0x30000, 0x30000, 0x10000);
    Call3(Func_02001890, -1, -1, 0xe666);
    Func_020018fe(163);
    bias = 4294967296.0;
    count = 0x1df;
    do {
        rec7 = Func_02001836();
        held = Func_02001c1c(*(volatile s32 *)(p10 + 36));
        shown = (s32)((u32)(rec7 << 11) >> 16);
        scaled = Func_02001c2a(shown);
        if (shown < 0) {
            bias = 4294967296.0;
            scaled = Func_02001b8a(scaled, bias);
        }
        Func_02001bde(held, Func_02001bd2(4718.592, scaled));
        *(volatile s32 *)(p10 + 36) = Func_02001cd6();
        Func_02001920(1);
        count = (count + -1);
    } while (count >= 0);
    c5 = 6;
    c6 = 6;
    count = 0;
    mode = (c5 << 10);
    do {
        Call2(Func_020018ba, 0x4000052, ((mode | (c5 << 5)) | c6));
        Func_02001948(1);
        if (Value2(Func_02001898, count, 20) == 0) {
            c6 = (c6 - 1);
            c5 = (c5 - 1);
        }
        count = (count + 1);
    } while (count <= 69);
    Call6(Func_02001926, 19, 83, 15, 8, 19, 91);
    Call1_02000754(Func_020019b6, 0x120);
    Func_0200191a();
    Func_0200195e();
    Func_0200199a();
}

void FieldScene_RunScene3a7SequenceB(void)
{
    u32 i;
    s32 record;
    s32 v5;
    s32 v6;

    if (Value1(Func_020019b0, 0x9a8) == 0) {
        Call2(Func_020019b4, 0x1528, 1);
        Call1_020008a8(Func_020019ca, 0x9a8);
        v5 = 27;
        v6 = 92;
        Func_02001a18(155);
        Call6(Func_020019a4, 107, 27, 1, 1, v6, v5);
        Func_020019fa(39);
        Call6(Func_020019ba, 108, 27, 1, 1, v6, v5);
        Func_02001a10(50);
        v6 = 25;
        Func_02001a4e(156);
        Call6(Func_020019d8, 1, 24, 1, 2, v6, v5);
        Func_02001a2e(40);
        Call6(Func_020019ee, 2, 24, 1, 2, v6, v5);
        Func_02001a44(40);
        Func_02001084();
    }
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    s32 i;
    u8 *rec7;
    s32 flag;
    s32 count;
    s32 index;
    volatile s32 *tick;
    volatile s32 *slot;
    u8 *table;

    flag = *(u8 *)(Func_02001af6(10) + 91);
    if (flag == 0) {
        tick = (volatile s32 *)Data_0200a224;
        count = *tick + 1;
        *tick = count;
        if (count > 190) {
            *tick = flag;
        }
        slot = (volatile s32 *)Data_0200a228;
        index = *slot;
        table = (u8 *)0x0200a214;
        if (*(s32 *)(table + (index << 2)) == *tick) {
            rec7 = Func_02001b22((index + 11));
            *(volatile s32 *)((s32)rec7 + 72) = 0xa3d;
            count = *slot + 1;
            *slot = count;
            if (count > 3) {
                *slot = flag;
            }
        }
        for (i = 0; i <= 3; i++) {
            rec7 = Value1(Func_02001b40, (i + 11));
            if (*(volatile s32 *)((s32)rec7 + 40) >= 0) {
                if (*(volatile s32 *)((s32)rec7 + 12) <= 0xffff) {
                    Func_020015b0();
                    *(volatile s32 *)((s32)rec7 + 12) = 0xff0000;
                    *(s32 *)((s32)rec7 + 72) = 0;
                    *(s32 *)((s32)rec7 + 40) = 0;
                    rec7[91] = 0;
                    Func_02001b8a_a(106);
                }
            }
        }
        if (Value1(Func_0200138a, 10) != 0) {
            Func_02001b92(10, 1);
            if (Value1(Func_02001b58, 0x207) == 0) {
                Call1_020009c0(Func_02001b6a, 0x207);
                Func_02001bb8(204);
            } else {
                Func_02001bc0(106);
            }
        }
        if (Value1(Func_020013ba, 9) != 0) {
            Func_02001bd0(106);
        }
    }
}

void Func_02000aa0(void)
{
    void Func_02001b8a();

    s32 i;
    u8 *rec7;
    s32 record;
    s32 count;
    volatile s32 *tick;
    s32 base5_200a230;

    rec7 = (u8 *)Value1(Func_02001bd6, 10);
    if (rec7[91] == 0) {
        tick = (volatile s32 *)Data_0200a22c;
        count = *tick + 1;
        *tick = count;
        if ((63 & count) == 0) {
            base5_200a230 = (s32)Data_0200a230;
            record = Func_02001b48();
            record = Value2(Func_02001b36, record, 6);
            *(volatile s32 *)base5_200a230 = record;
            rec7 = Func_02001c06((record + 10));
            *(volatile s32 *)((s32)rec7 + 72) = 0xa3d;
        }
        for (i = 0; i <= 5; i++) {
            rec7 = Func_02001c1a((i + 10));
            record = Value1(Func_02001bf6, (i + 0x200));
            if (record != 0) {
                if (*(volatile s32 *)((s32)rec7 + 40) <= 0) {
                    if (*(volatile s32 *)((s32)rec7 + 12) > 0x20ffff) {
                        continue;
                    }
                }
                *(volatile s32 *)((s32)rec7 + 12) = 0xff0000;
                *(s32 *)((s32)rec7 + 72) = 0;
                *(s32 *)((s32)rec7 + 40) = 0;
                Func_02001c66(106);
            } else {
                if (*(volatile s32 *)((s32)rec7 + 40) <= 0) {
                    if (*(volatile s32 *)((s32)rec7 + 12) > 0xffff) {
                        continue;
                    }
                }
                *(s32 *)((s32)rec7 + 72) = record;
                *(s32 *)((s32)rec7 + 40) = record;
                *(volatile s32 *)((s32)rec7 + 12) = 0xff0000;
                Func_02001c82(106);
            }
        }
    }
}
