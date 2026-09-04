#include "types.h"

#define FieldScene_RunScene37a_02001264 Func_02001264

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02003c20();
void Func_02003c32();
void Func_02003c42();
void Func_02003c54();
void Func_02003c5c();
void Func_02003c60();
void Func_02003cc0();
void Func_02003cd0();
void Func_02003cd6();
void Func_02003cfc();
void Func_02003d02();
void Func_02003d06();
void Func_02003d14();
void Func_02003d1c();
void Func_02003d6c();
void Func_02003db4();
void Func_02003dca();
void Func_02003e00();
s32 Func_02003e14();
void Func_02003e16();
void Func_02003e2a();
void Func_02003e30();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene37a_02001264(void)
{
    u32 i;
    s32 record;
    u8 *work;

    Call4(Func_02003d14, -1, -1, -1, 0);
    Call6(Func_02003c20, 30, 43, 32, 40, 8, 3);
    Call6(Func_02003c32, 30, 43, 33, 39, 8, 1);
    Call6(Func_02003c42, 30, 43, 36, 38, 3, 3);
    Call6(Func_02003c54, 14, 41, 32, 41, 8, 4);
    Call4(Func_02003d6c, 0x23e0000, -1, 0x9e0000, 0);
    Func_02003c60();
    Call3(Func_02003cfc, 16, 0x23e0000, 0x780000);
    Func_02003d06(0, 0, 0);
    Func_02003c5c(1);
    Call2(Func_02003db4, 0x2051cc, 1);
    Func_02003dca(20);
    Call1(Func_02003cc0, 0x201);
    Call1(Func_02003cd0, 0x200);
    Call1(Func_02003cd6, 0x202);
    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0x100;
    *(volatile s32 *)(((s32)work + 0x1c8)) = 32;
    Func_02003e00();
    ((void (*)())Func_02003e14)();
    Func_02003d02(40);
    Func_02003e30(171);
    Call2(Func_02003e00, 0x10005, 1);
    Func_02003e16(8);
    Func_02003d1c(32);
    Value2(Func_02003e14, 0x2051cc, 1);
    Func_02003e2a(24);
}
