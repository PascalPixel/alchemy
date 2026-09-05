#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020034b4();
void Func_020068d8();
s32 Func_020068fa();
void Func_02006900();
s32 Func_02006930();
void Func_0200693c();
void Func_0200695a();
void Func_02006964();
void Func_0200696e();
void Func_02006974();
void Func_02006a32();
s32 Func_02006a3c();
void Func_02006a3e();
void Func_02006a4a();
void Func_02006a56();
void Func_02006a60();
void Func_02006a64();
void Func_02006a70();

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

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_020068fa, 0);
    Func_020068d8();
    Func_0200695a(10, 0, 0);
    Func_02006964(8, 0, 0);
    *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0) = 0x201;
    Func_02006a3e();
    Func_02006a4a();
    Func_02006900(20);
    Func_02006a60(202, 3);
    Value2(Func_02006930, 202, 0);
    *(u8 *)(Func_02006a3c() + 85) = 0;
    Call2(Func_02006a32, 0x19999, 0x3333);
    Call4(Func_02006a4a, 0x640000, 0, 0xf90000, 1);
    Func_02006a56();
    Func_0200693c(20);
    Func_020034b4();
    Func_02006a64(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), *(s32 *)(rec7 + 16), 1);
    Func_02006a70();
    Call1(Func_0200696e, 0x200cd6c);
    *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0) = 0x204;
    Func_02006974();
}
