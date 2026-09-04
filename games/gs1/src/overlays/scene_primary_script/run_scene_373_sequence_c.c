#include "types.h"

#define FieldScene_RunScene373SequenceC Func_02000fec

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02006eb4();
void Func_02006fbc();
s32 Func_02006fd6();
s32 Func_02006fde();
void Func_02007022();
void Func_02007030();
void Func_02007064();
s32 Func_0200706e();
void Func_0200707e();
void Func_020070a8();
void Func_020070be();
void Func_020070c2();
void Func_020070c8();
void Func_020070d4();
void Func_020070d6();
void Func_020070da();
void Func_020070e8();
void Func_020070fe();
void Func_0200710a();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene373SequenceC(void)
{
    u32 i;
    s32 rec7;
    s32 rec8;
    s32 record;

    rec8 = Value1(Func_02006fd6, 0);
    rec7 = Value1(Func_02006fde, 5);
    Func_02006fbc();
    *(volatile s32 *)(rec7 + 8) = *(volatile s32 *)(rec8 + 8);
    *(volatile s32 *)(rec7 + 12) = *(volatile s32 *)(rec8 + 12);
    *(volatile s32 *)(rec7 + 16) = *(volatile s32 *)(rec8 + 16);
    *(volatile s32 *)(rec7 + 56) = -0x80000000;
    *(volatile s32 *)(rec7 + 60) = -0x80000000;
    *(volatile s32 *)(rec7 + 64) = -0x80000000;
    *(volatile s32 *)(rec7 + 36) = 0;
    *(volatile s32 *)(rec7 + 40) = 0;
    *(volatile s32 *)(rec7 + 44) = 0;
    *(volatile s32 *)(rec7 + 20) = *(volatile s32 *)(rec8 + 12);
    Func_02006eb4(1);
    Call3(Func_02007022, 5, 0x10000, 0x8000);
    Call3(Func_02007064, 5, 110, 0x11b);
    Func_020070be(0, 5, 2);
    Call1(Func_020070d4, 0xf39);
    if (*(volatile s32 *)(rec8 + 8) < *(volatile s32 *)(rec7 + 8)) {
        Call3(Func_020070fe, 0xa005, 0, 2);
    } else {
        Call3(Func_0200710a, 0x8005, 0, 2);
    }
    Func_020070c2(0, 3);
    Func_02007030(2);
    Func_020070c8(5, 2);
    record = Value1(Func_0200706e, 0);
    if (record != 0) {
        Func_020070a8(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_020070d6(5);
    Func_020070e8(5, 0, 0);
    Call3(Func_020070da, 0, 110, 0x12f);
    Func_0200707e();
}
