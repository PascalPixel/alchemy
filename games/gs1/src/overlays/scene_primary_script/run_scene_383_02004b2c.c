#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200e4f8[];
void Func_02006a5c();
void Func_02009874();
void Func_020098a8();
void Func_020098d0();
void Func_020098da();
void Func_020098e4();
void Func_02009920();
void Func_02009940();
void Func_02009946();
void Func_0200994c();
void Func_02009954();
void Func_0200995e();
void Func_02009960();
void Func_02009976();
void Func_02009982();
void Func_0200998a();
void Func_02009990();
void Func_02009994();
void Func_02009996();
void Func_0200999c();
void Func_020099aa();
void Func_020099ac();
void Func_020099ea();
void Func_020099f6();
void Func_02009a54();
void Func_02009b68();

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void FieldScene_RunScene383_02004b2c(void)
{
    Func_02009874();
    Call4(Func_02009996, 0xa80000, -1, 0x2900000, 1);
    Call3(Func_020098d0, 0, 0xcccc, 0x6666);
    Call3(Func_020098da, 1, 0xcccc, 0x6666);
    Call3(Func_020098e4, 2, 0xcccc, 0x6666);
    Call3(Func_02009920, 0, 248, 0x2b8);
    Call3(Func_02009946, 1, 0xf80000, 0x2b80000);
    Call3(Func_02009954, 2, 0xf80000, 0x2b80000);
    Call3(Func_02009940, 0, 200, 0x2b8);
    Call3(Func_0200994c, 1, 248, 0x2c8);
    Call3(Func_02009960, 2, 232, 0x2b8);
    Func_02009976(1);
    Call3(Func_020099ea, 1, 0xc000, 0);
    Call3(Func_020099f6, 2, 0x8000, 0);
    Func_02009994(0);
    Func_020099ac(1, 12);
    Func_02006a5c();
    Call4(Func_02009a54, 0x300000, 0x2400000, 0x1200000, 0x2e00000);
    Call3(Func_02009982, 1, 0x10000, 0x8000);
    Call3(Func_02009990, 2, 0xc000, 0x6000);
    Call3(Func_0200999c, 24, 0x10000, 0x13333);
    Call3(Func_020099aa, 25, 0x18000, 0x18000);
    *(volatile u16 *)Data_0200e4f8 = 0;
    Call2(Func_020098a8, 0x200c8c9, 0xc94);
    Func_0200995e(0x1ff);
    Func_0200998a();
    Func_02009b68(9);
}
