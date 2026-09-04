#include "types.h"

#define FieldScene_RunScene3a0_02000968 Func_02000968

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001cf8();
void Func_02001d2e();
s32 Func_02001d44();
s32 Func_02001d54();
s32 Func_02001d60();
void Func_02001d66();
s32 Func_02001d68();
void Func_02001d92();
void Func_02001e4e();

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

void FieldScene_RunScene3a0_02000968(void)
{
    u32 i;
    s32 record;
    s32 v5;
    u8 *p5;

    Func_02001d2e();
    *(u8 *)(Func_02001d44(20) + 35) &= 253;
    v5 = 0;
    *(u8 *)(Func_02001d54(20) + 85) = v5;
    record = Value1(Func_02001d60, 20);
    p5 = *(volatile s32 *)(record + 8);
    record = Value1(Func_02001d68, 20);
    Call6(Func_02001d2e, 3, 17, 1, 1, ((s32)p5 >> 20), (*(volatile s32 *)(record + 16) >> 20));
    Call2(Func_02001cf8, 0x2008325, 0xc80);
    Call1(Func_02001d66, 0x201);
    Func_02001e4e(20, 2);
    Func_02001d92();
}
