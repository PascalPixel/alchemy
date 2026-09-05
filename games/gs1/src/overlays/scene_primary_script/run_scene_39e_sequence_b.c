#include "types.h"

#define FieldScene_RunScene39eSequenceB Func_02000658

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02004a28();
void Func_02004a3a();
void Func_02004a48();
void Func_02004a5c();
void Func_02004a76();
void Func_02004a9c();
void Func_02004aa4();
void Func_02004abe();
void Func_02004aea();
void Func_02004af0();
void Func_02004af6();
void Func_02004b16();
void Func_02004b36();
void Func_02004b40();
void Func_02004b58();
void Func_02004b7a();
void Func_02004b7e();
void Func_02004b86();
void Func_02004ba0();
void Func_02004ba8();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

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
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene39eSequenceB(void)
{
    u32 i;
    s32 record;

    Func_02004a3a();
    Call1(Func_02004a28, 0x894);
    Func_02004aea(9, 0, 0);
    Func_02004a48(10);
    Call1(Func_02004b16, 0x17b7);
    Func_02004af6(9, 2);
    Func_02004a5c(20);
    Call3(Func_02004b58, 0, 0x8000, 20);
    Func_02004b58(9, 0);
    Func_02004a76(10);
    Call3(Func_02004b7a, 9, 0x100, 80);
    Call3(Func_02004b7e, 9, 0xd000, 20);
    Func_02004b36(9, 2);
    Func_02004a9c(20);
    Func_02004b86(9, 0, 20);
    Func_02004ba0(9, 0, 20);
    Func_02004b40(9, 3);
    Func_02004abe(20);
    Func_02004ba8(9, 0, 20);
    Call6(Func_02004aa4, 10, 26, 1, 1, 10, 24);
    Func_02004af0();
}
