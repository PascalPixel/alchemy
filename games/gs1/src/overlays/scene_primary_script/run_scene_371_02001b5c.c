#include "types.h"

#define FieldScene_RunScene371_02001b5c Func_02001b5c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02005d04();
void Func_02005d26();
void Func_02005e44();
s32 Func_02005e5e();
void Func_02005e64();
void Func_02005e72();
void Func_02005ec4();
void Func_02005ee6();
void Func_02005ef0();
void Func_02005f06();
void Func_02005f6e();
void Func_02005f8e();
void Func_02005fe8();
void Func_02005fea();
void Func_0200600a();
void Func_02006016();

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

void FieldScene_RunScene371_02001b5c(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_02005e5e, 8);
    Func_02005e44();
    Call4(Func_02005f6e, -1, -1, -1, 0);
    Func_02005d04(1);
    Func_02005ee6(0, 0, 0);
    Call3(Func_02005ef0, 8, 0x13e80000, 0x9180000);
    *(s32 *)(rec7 + 28) = 0x14000;
    *(s32 *)(rec7 + 24) = 0x14000;
    Func_02005d26(1);
    Func_02005f8e(8, 1);
    Func_02005fea();
    Call3(Func_02005ec4, 8, 0x6666, 0x3333);
    Call3(Func_02005f06, 8, 0x13c8, 0x918);
    Func_0200600a();
    Func_02006016();
    Call1(Func_02005e64, 0x93e);
    Call1(Func_02005e72, 0x927);
    Func_02005fe8(107);
    Func_02005ec4();
}
