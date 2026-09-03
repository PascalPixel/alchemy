#include "types.h"

#define FieldScene_RunOpeningAuxiliarySequence Func_020001d0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
s32 Func_02001062();
void Func_0200107c();
s32 Func_02001094();
u8 *Func_020010e0();
void Func_020010f6();
void Func_020010fc();
void Func_02001114();
u8 *Func_0200112a();
void Func_0200112c();
void Func_02001130();
void Func_0200113c();
void Func_0200114e();
void Func_0200115e();
void Func_02001184();
void Func_0200119e();
s32 Func_020011a6();
void Func_020011aa();
void Func_020011b2();
void Func_020011d6();
void Func_020011e8();
void Func_020011f4();

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    u8 *rec7;
    u8 *rec8;
    u8 *record;

    Func_0200107c();
    if (Value1(Func_02001062, 0x909) != 0) {
        Call1(Func_02001114, 0x191f);
        Func_0200113c(14, 0);
    } else {
        Func_020010f6(14, 4);
        Call1(Func_0200112c, 0x18c7);
        Func_0200114e(14, 0, 10);
        rec8 = Value1(Func_02001094, 0x8ff);
        if (rec8 == 0) {
            rec7 = Func_020010e0(18);
            *(u8 *)(Func_020011a6() + 85) = rec8;
            Call2(Func_0200119e, 0x10000, 0x2000);
            Func_020011b2(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), *(s32 *)(rec7 + 16), 1);
            Call3(Func_0200115e, 0, 0x4000, 0);
            Call3(Func_020011aa, 14, 0x3000, 0);
            Func_020011d6();
            Func_020010fc(120);
            record = Func_0200112a(0);
            Func_020011e8(*(s32 *)(record + 8), *(s32 *)(record + 12), *(s32 *)(record + 16), 1);
            Func_020011f4();
        }
        Func_02001184(14, 4);
    }
    Func_02001130();
}
