#include "types.h"

#define FieldScene_RunOpeningAuxiliarySequence Func_020007d8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
s32 Func_0200565a();
void Func_02005676();
void Func_02005684();
u8 *Func_0200568a();
void Func_020056b0();
u8 *Func_020056d8();
void Func_02005702();
u8 *Func_02005712();
void Func_02005720();
void Func_0200575a();
void Func_0200578a();
void Func_02005794();
void Func_020057fa();
void Func_0200580e();
void Func_0200583c();
void Func_0200585a();
void Func_02005874();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
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

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    u8 *rec7;
    u8 *rec8;
    u8 *record;

    rec7 = Func_0200568a(0);
    rec8 = Value1(Func_0200565a, 0x109);
    if (rec8 == 0) {
        Func_02005684();
        Call4(Func_0200578a, -1, -1, -1, 0);
        rec7[85] = rec8;
        Func_02005712(0, (*(s16 *)(rec7 + 10) << 16), ((*(s16 *)(rec7 + 18) << 16) + -0x100000));
        Func_0200575a(0, 15);
        record = Func_020056d8(0);
        Func_02005676(record, 0);
        Func_020057fa();
        Func_0200580e();
        Func_0200583c(228);
        *(s32 *)(rec7 + 108) = 0x20086a1;
        Call3(Func_02005702, 0, 0x6666, 0x3333);
        Func_0200583c(0, 0, 8);
        Func_02005794(0, 0);
        record = Func_02005712(0);
        Func_020056b0(record, 1);
        Func_0200585a(0, 0, 8);
        rec7[85] = 3;
        *(s32 *)(rec7 + 108) = rec8;
        Func_02005874();
        Func_02005720();
    }
}
