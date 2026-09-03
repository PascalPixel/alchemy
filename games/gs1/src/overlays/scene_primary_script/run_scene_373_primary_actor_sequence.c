#include "types.h"

#define FieldScene_RunPrimaryActorSequence Func_02002f14

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00001c60[];
void Func_02008ed4();
s32 Func_02008f26();
void Func_02008f54();
void Func_02008f80();
void Func_02008f8c();
void Func_02008fe2();
void Func_02008fe8();
void Func_02008ff2();
void Func_02009004();
void Func_0200900a();
void Func_02009014();
void Func_02009016();
void Func_02009034();
void Func_02009042();
void Func_0200904a();
void Func_0200904e();
void Func_02009054();
void Func_02009056();
void Func_0200905a();
void Func_02009080();
void Func_02009082();
void Func_02009088();
void Func_0200909e();
s32 Func_020090aa();
void Func_020090b0();
void Func_020090bc();
void Func_020090c0();
void Func_020090cc();
void Func_020090d0();
void Func_020090d8();
void Func_020090e6();
void Func_020090f4();
void Func_02009100();
void Func_02009108();
void Func_0200910c();
void Func_0200911a();
void Func_0200912a();
void Func_0200913c();
s32 Func_02009140();
void Func_0200914e();
s32 Func_02009152();
void Func_0200915a();
void Func_02009166();
void Func_0200916a();
void Func_02009178();
void Func_0200917a();
void Func_0200918e();
void Func_02009192();
void Func_02009194();
void Func_0200919a();
void Func_020091a0();
void Func_020091a6();
void Func_020091a8();
void Func_020091b0();
void Func_020091ba();
void Func_020091c0();
void Func_020091ea();

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunPrimaryActorSequence(void)
{
    u32 i;
    s32 record;

    Func_02008ed4();
    Call4(Func_0200900a, 0x1650000, -1, 0x2e20000, 1);
    Call3(Func_02008f54, 0, 0x16f, 0x2e9);
    Call3(Func_02008fe8, 0, 0xa000, 0);
    record = Value1(Func_02008f26, 0);
    if (record != 0) {
        Func_02008f8c(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Call3(Func_02008f80, 1, 0x15a, 0x2e9);
    Call3(Func_02009014, 1, 0xd000, 20);
    Call1(Func_02008ff2, 0x1c53);
    Func_0200900a(1, 0);
    Func_02008fe2(9, 2);
    Call3(Func_0200904e, 9, 0x100, 0);
    Call3(Func_02009042, 9, 0x3000, 10);
    Call3(Func_0200904e, 9, 0x5000, 10);
    Call3(Func_0200905a, 9, 0x3000, 40);
    Func_02009054(9, 0, 20);
    Func_02009004(0, 3);
    Func_02009014(1, 3);
    Func_02009034(9, 1);
    Call3(Func_02009088, 9, 0x5000, 10);
    Func_02009082(9, 0, 20);
    Func_0200904a(1, 1);
    Call3(Func_020090bc, 1, 0x103, 40);
    Func_0200909e(1, 0, 10);
    Func_02009056(9, 3);
    Func_020090b0(9, 0, 20);
    Call3(Func_020090cc, 0, 0x8000, 0);
    Call3(Func_020090d8, 1, 0x1000, 40);
    Func_02009080(9, 4);
    Func_020090d0(9, 0);
    Call3(Func_020090f4, 0, 0xb000, 0);
    Call3(Func_02009100, 1, 0xd000, 10);
    Func_020090c0(1, 2);
    Func_02009016(20);
    Func_02009108(1, 0, 10);
    Func_020090c0(9, 3);
    Func_0200911a(9, 0, 20);
    Call3(Func_0200914e, 0, 0x102, 0);
    Call3(Func_0200915a, 1, 0x102, 80);
    Func_0200913c(1, 0, 20);
    Func_0200910c(9, 2);
    Func_0200914e(9, 0, 20);
    Call3(Func_0200916a, 1, 0x1000, 20);
    Value2(Func_02009152, 1, 0);
    if (Value2(Func_020090aa, 0, 0) == 0) {
        Call3(Func_020091a0, 1, 0x105, 60);
    } else {
        bump_step(1);
    }
    Func_02009194(1, 0, 20);
    Call3(Func_020091b0, 1, 0xd000, 10);
    Func_0200918e((s32)Data_00001c60);
    Func_020091a6(1, 0);
    Func_02009166(9, 3);
    Func_020091c0(9, 0, 20);
    Func_02009178(1, 3);
    Func_020090e6(20);
    Call3(Func_020091ea, 1, 0x1000, 20);
    Func_02009192(1, 3);
    Func_0200919a(0, 3);
    Func_0200919a(1, 2);
    record = Value1(Func_02009140, 0);
    if (record != 0) {
        Func_0200917a(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_020091a8(1);
    Func_020091ba(1, 0, 0);
    Call1(Func_0200912a, 0x304);
    Func_0200914e();
}
