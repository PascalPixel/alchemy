#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
s32 Func_020078d6();
void Func_02007986();
s32 Func_0200798c();
void Func_020079e6();
s32 Func_020079f2();
void Func_02007a16();
s32 Func_02007a1c();
void Func_02007a38();
void Func_02007a5e();
void Func_02007a74();
void Func_02007a7e();
void Func_02007a8c();
void Func_02007abc();
void Func_02007aee();
void Func_02007be2();

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

void FieldScene_RunOpeningAuxiliarySequence(s32 a0)
{
    s32 rec2;
    u8 *rec7;
    s32 record;
    u8 *p6;
    u8 *base;

    base = Data_02000240;
    p6 = *(volatile s32 *)(base + 500);
    rec7 = Value1(Func_020079f2, (s32)p6);
    rec2 = Value1(Func_0200798c, 0x2f0);
    if (rec2 == 0) {
        Func_020079e6();
        Call2(Func_02007aee, (s32)p6, 0x101);
        Func_02007a7e((s32)p6, 9);
        record = Value1(Func_02007a1c, a0);
        if (record != 0) {
            Func_02007a5e((s32)p6, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_02007a8c((s32)p6);
        Func_02007be2(244);
        Value2(Func_020078d6, 0x200b679, 0xc80);
        rec7[85] = rec2;
        {
            s32 shifted = *(volatile s32 *)((s32)rec7 + 12) + 0x200000;

            Func_02007986((s32)rec7, *(volatile s32 *)((s32)rec7 + 8), shifted, *(volatile s32 *)((s32)rec7 + 16));
        }
        Func_02007abc((s32)p6);
        *(volatile s32 *)((s32)rec7 + 40) = rec2;
        rec7[85] = 4;
        *(u8 *)(base + 498) = 2;
        Call1(Func_02007a16, 0x2f0);
        Call2(Func_02007a38, 0x2f8, 180);
        Func_02007a74();
        *(u16 *)((*(u8 *volatile *)Data_03001ebc + 0x17c)) = rec2;
    }
}
