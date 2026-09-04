#include "types.h"

#define FieldScene_RunScene383_0200091c Func_0200091c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02004f4e();
s32 Func_02005644();
void Func_02005664();
void Func_020056a0();
void Func_020056ae();
s32 Func_020056b8();
void Func_020056bc();
s32 Func_020056ce();
void Func_020056d2();
void Func_020056d8();
void Func_02005710();
void Func_0200571e();
void Func_02005722();
void Func_02005746();
void Func_0200574e();
void Func_0200575e();
void Func_0200576c();
void Func_0200576e();
void Func_02005776();
void Func_02005790();
void Func_020057b0();
void Func_020057b8();
void Func_020057c8();
void Func_0200580e();
void Func_02005822();
void Func_0200586a();

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
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

void FieldScene_RunScene383_0200091c(void)
{
    u32 i;
    s32 record;

    Func_02005664();
    Func_02004f4e(18, 0, 2);
    if (Value1(Func_02005644, 0x85b) == 0) {
        Call1(Func_0200574e, 0x137c);
        Func_0200575e(18, 0);
    } else {
        Call1(Func_0200575e, 0x1385);
        Func_0200576e(18, 0);
    }
    if (Value2(Func_020056ce, 0, 0) == 0) {
        Func_020056a0(20);
        Func_02005790(18, 0);
        Func_020056ae(20);
        Func_02005776(18, 2);
        Func_020056bc(20);
        if (Value0(Func_020056b8) == 0) {
            Func_0200576c(18, 4);
            Func_020056d2(20);
            Call1(Func_020057b0, 0x1384);
            Func_020057c8(18, 0);
            goto L_020009ec;
        }
        Func_0200586a(231, 3);
        Func_02005722(231, 0);
        Call1(Func_020056d8, 0x85b);
    } else {
        bump_step(1);
        Func_02005710(20);
        Func_020057b8(18, 3);
        Func_0200571e(20);
        Func_0200580e(18, 0);
    }
    L_020009ec:;
    Call3(Func_02005822, 18, 0x4000, 0);
    Func_02005746();
}
