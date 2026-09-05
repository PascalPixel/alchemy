#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001c9e();
void Func_02001cb4();
void Func_02001cca();
void Func_02001cd0();
void Func_02001ce6();
void Func_02001cfc();
void Func_02001d06();
void Func_02001d80();
void Func_02001d8c();
void Func_02001da2();
void Func_02001da6();
void Func_02001dc2();
void Func_02001dce();
void Func_02001e12();
void Func_02001e2c();
void Func_02001e7e();
void Func_02001e8c();
void Func_02001e98();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
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

void FieldScene_RunScene37fSequenceA(void)
{
    u32 i;
    s32 record;
    s32 v5;
    s32 v6;

    Func_02001d06();
    v5 = 3;
    v6 = 2;
    Func_02001e2c(181);
    Call6(Func_02001cd0, 16, 28, 21, 3, v5, v6);
    Func_02001c9e(10);
    Call6(Func_02001ce6, 16, 30, 21, 3, v5, v6);
    Func_02001cb4(10);
    Call6(Func_02001cfc, 16, 32, 21, 3, v5, v6);
    Func_02001cca(10);
    Func_02001e12(0, 2);
    Call3(Func_02001d8c, 0, 0x9999, 0x4ccc);
    Func_02001da6(0, 120, 98);
    Func_02001dce(0, 2);
    Call3(Func_02001dc2, 0, 0, -8);
    Func_02001d80(10);
    Func_02001e8c();
    Func_02001e98();
    Func_02001e7e(2);
    Func_02001da2();
}
