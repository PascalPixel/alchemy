#include "types.h"

#define FieldScene_RunScene3bfSequenceD Func_02000dcc

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001b44();
s32 Func_020063de();
void Func_020063f6();
void Func_02006400();
void Func_02006432();
void Func_0200659e();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void FieldScene_RunScene3bfSequenceD(void)
{
    u8 *p5;
    s16 field;

    p5 = *(volatile s32 *)Data_03001ebc;
    if (Func_020063de(234) != -1) {
        field = *(s16 *)((s32)p5 + 0x16c);
        Func_02001b44(field - 40);
        Func_0200659e(157);
        Call3(Func_020063f6, 0x30000, 0x30000, 0x10000);
        Call3(Func_02006400, -1, -1, 0xe666);
        Func_02006432(field + 0x32d);
    }
}
