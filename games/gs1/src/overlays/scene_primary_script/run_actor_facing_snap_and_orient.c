#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_0200145a();
void Func_02003fc8();
void Func_02003fca();
void Func_02003ff2();
s32 Func_02004002();
void Func_02004006();
s32 Func_02004024();
void Func_0200403e();
void Func_02004046();
void Func_02004058();
void Func_02004076();
s32 Func_020040de();
void Func_020041de();

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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

void FieldScene_RunActorFacingSnapAndOrient(void)
{
    s32 p8;
    u8 *rec7;
    s32 angle;
    s16 buf[6];

    rec7 = Value1(Func_02004024, 0);
    angle = (*(volatile u16 *)((s32)rec7 + 6) + 0x1000) & 0xe000;
    p8 = rec7[85];
    {
        s32 *w = (s32 *)buf;
        w[0] = (*(volatile s32 *)((s32)rec7 + 8) & -0x100000) + 0x80000;
        w[1] = *(volatile s32 *)((s32)rec7 + 12);
        w[2] = (*(volatile s32 *)((s32)rec7 + 16) & -0x100000) + 0x80000;
    }
    Call3(Func_02003fca, 0x200000, angle, (s32)buf);
    if (Value2(Func_02004002, (s32)rec7, (s32)buf) == 0) {
        Call1(Func_02004046, 0x250);
        Func_0200145a();
        Func_02003ff2((s32)rec7, 6);
        Func_02003fc8(6);
        Func_020041de(152);
        Func_02004006((s32)rec7, 7);
        *(volatile s32 *)((s32)rec7 + 48) = 0x30000;
        *(volatile s32 *)((s32)rec7 + 52) = 0x20000;
        *(volatile s32 *)((s32)rec7 + 40) = 0x40000;
        rec7[85] &= 126;
        Func_02004058((s32)rec7, 0);
        Value3(Func_020040de, 0, buf[1], buf[5]);
        Func_0200403e((s32)rec7, 6);
        Func_02004076((s32)rec7, 1);
        rec7[85] = p8;
    }
}
