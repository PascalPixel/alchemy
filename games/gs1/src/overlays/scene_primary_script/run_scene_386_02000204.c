#include "types.h"

#define FieldScene_RunScene386_02000204 Func_02000204

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_0200082c();
void Func_02000844();
void Func_02000880();
void Func_0200088e();
s32 Func_02000898();
void Func_0200089c();
s32 Func_0200089e();
void Func_020008b2();
void Func_020008b6();
void Func_020008c0();
void Func_020008c6();
void Func_020008d6();
void Func_020008e6();
void Func_020008f0();
void Func_020008f2();
void Func_020008f4();
void Func_020008f8();
void Func_020008fe();
void Func_02000918();
void Func_02000926();
void Func_02000930();
void Func_02000940();
void Func_0200095a();
void Func_02000976();
void Func_02000992();

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

void FieldScene_RunScene386_02000204(void)
{
    u32 i;
    s32 record;

    Func_02000844();
    ((void (*)())Func_0200089e)(18, 0, 0);
    if (Value1(Func_0200082c, 0x85b) == 0) {
        Call1(Func_020008b6, 0x137c);
        Func_020008c6(18, 0);
    } else {
        Call1(Func_020008c6, 0x1385);
        Func_020008d6(18, 0);
    }
    if (Value2(Func_0200089e, 0, 0) == 0) {
        Func_02000880(20);
        Func_020008f8(18, 0);
        Func_0200088e(20);
        Func_020008e6(18, 2);
        Func_0200089c(20);
        if (Value0(Func_02000898) == 0) {
            Func_020008f4(18, 4);
            Func_020008b2(20);
            Call1(Func_02000918, 0x1384);
            Func_02000930(18, 0);
            goto L_020002d4;
        }
        Func_0200095a(231, 3);
        Func_020008f2(231, 0);
        Call1(Func_020008c0, 0x85b);
    } else {
        bump_step(1);
        Func_020008f0(20);
        Func_02000940(18, 3);
        Func_020008fe(20);
        Func_02000976(18, 0);
    }
    L_020002d4:;
    Call3(Func_02000992, 18, 0x4000, 0);
    Func_02000926();
}
