#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_02000754

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
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
double Func_02001b8a();
double Func_02001bd2();
void Func_02001bde();
double Func_02001c1c();
double Func_02001c2a();
s32 Func_02001cd6();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
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

void FieldScene_RunSupplementalSequenceOne(void)
{
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
    Call1(Func_020019b6, 0x120);
    Func_0200191a();
    Func_0200195e();
    Func_0200199a();
}
