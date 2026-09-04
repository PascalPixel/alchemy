#include "types.h"

#define FieldScene_RunScene39f_020021b0 Func_020021b0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02002cf0();
void Func_02002f74();
void Func_02002fce();
void Func_02002ff0();
void Func_0200300e();
void Func_0200307c();
void Func_02004f62();
s32 Func_02004f74();
void Func_02004f86();
void Func_02004fc0();
void Func_02004fec();
void Func_0200500c();
void Func_0200502a();
void Func_02005032();
void Func_02005040();
void Func_02005042();
void Func_02005048();
void Func_0200505c();
void Func_0200506a();
void Func_0200507e();
void Func_02005082();
void Func_02005094();
void Func_0200509c();
void Func_020050ea();

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

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene39f_020021b0(void)
{
    s32 rec7;

    rec7 = Value1(Func_02004f74, 18);
    Func_02004f62();
    Call3(Func_02004fc0, 18, 0x880000, 0x1680000);
    Func_0200307c(18, 1);
    Func_02002f74(18, 136, 0x198, 0x80000);
    Func_02004f86(10);
    Func_02002cf0(*(s32 *)(rec7 + 8), *(s32 *)(rec7 + 12), (*(s32 *)(rec7 + 16) + 0x40000), 0, 0, 0, 1, 0);
    Call3(Func_02005048, 18, 0xc000, 40);
    Call2(Func_0200506a, 18, 0x102);
    Func_02005032(18, 2);
    Func_02005082(18, 1);
    Func_02002fce(18, 136, 0x1b8, 0x60000);
    Func_0200505c(0, 18, 0);
    Func_02004fec(10);
    Func_02002ff0(18, 136, 0x1d8, 0x30000);
    Func_0200507e(0, 18, 0);
    Func_0200500c(6);
    Func_0200300e(18, 136, 0x1f8, 0x30000);
    Func_0200509c(0, 18, 0);
    Func_0200502a(6);
    Func_020050ea(0, 1);
    Func_02005094(18, 0, 0);
    Func_02005042(60);
    Call1(Func_02005040, 0x89d);
    Func_0200505c();
}
