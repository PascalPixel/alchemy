#include "types.h"

#define FieldScene_RunMiddleAuxiliarySequence Func_02000464

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
s32 Func_020016d4();
s32 Func_020016da();
s32 Func_0200170c();
void Func_02001714();
void Func_0200171c();
s32 Func_0200173a();
void Func_02001762();
void Func_0200176c();
void Func_0200177c();
void Func_02001784();
void Func_0200178a();
void Func_0200178e();
void Func_020017aa();
void Func_020017ba();
void Func_020017c4();
void Func_02001840();

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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunMiddleAuxiliarySequence(s32 a0)
{
    s32 p10;
    s32 rec2;
    u8 *rec7;
    s32 record;
    u8 *p6;
    u8 *base;

    base = Data_02000240;
    p6 = *(volatile s32 *)(base + 500);
    p10 = a0;
    rec7 = Value1(Func_0200170c, (s32)p6);
    Func_02001714(p10);
    rec2 = Value1(Func_020016da, 0x20f);
    if (rec2 == 0) {
        Func_02001714();
        Call2(Func_020017c4, (s32)p6, 0x101);
        Func_0200177c((s32)p6, 9);
        record = Value1(Func_0200173a, p10);
        if (record != 0) {
            Func_0200176c((s32)p6, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_0200178a((s32)p6);
        Func_02001840(244);
        Value2(Func_020016d4, 0x2008401, 0xc80);
        rec7[85] = rec2;
        {
            s32 v12 = *(volatile s32 *)((s32)rec7 + 12) + 0x200000;
            Func_0200171c((s32)rec7, *(volatile s32 *)((s32)rec7 + 8), v12, *(volatile s32 *)((s32)rec7 + 16));
        }
        Func_020017ba((s32)p6);
        *(volatile s32 *)((s32)rec7 + 40) = rec2;
        rec7[85] = 4;
        *(u8 *)(base + 498) = 2;
        Call1(Func_02001762, 0x20f);
        Call2(Func_02001784, 0x218, p10);
        Call2(Func_0200178e, 0x210, 180);
        Func_020017aa();
        *(u16 *)((*(u8 *volatile *)Data_03001ebc + 0x17c)) = rec2;
    }
}
