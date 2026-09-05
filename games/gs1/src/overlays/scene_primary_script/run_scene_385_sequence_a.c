#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02000e04();
s32 Func_02001970();
void Func_020019a2();
void Func_020019bc();
s32 Func_020019c0();
s32 Func_020019f4();
void Func_020019fe();
void Func_02001a06();
s32 Func_02001a1e();
void Func_02001a3a();
s32 Func_02001a44();
void Func_02001a4a();
s32 Func_02001a56();
s32 Func_02001a64();
void Func_02001a90();
void Func_02001aa6();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call0(void (*f)())
{
    f();
}

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

void FieldScene_RunScene385SequenceA(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 v5;

    rec7 = Value1(Func_02001970, 0x308);
    if (rec7 == 0) {
        Func_020019a2();
        *(u8 *)(Func_020019c0(16) + 91) = 1;
        Func_02001a06(16, 1);
        Call2((void (*)())Func_02001a1e, 16, 1);
        Func_020019bc(20);
        Call1(Func_02001a4a, 0x1cb5);
        Call3((void (*)())Func_02001a44, 16, 0, 2);
        Value2(Func_02001a64, 16, 0);
        if (Value2(Func_020019f4, 0, 0) != 0) {
            bump_step(1);
        }
        Func_02001a90(16, 0);
        *(u8 *)(Func_02001a1e(16) + 91) = rec7;
        Func_02001a3a(16, 2);
        Call0((void (*)())Func_02001a1e);
        Call1(Func_020019fe, 0x308);
    } else {
        Call1(Func_02001aa6, 0x1cc2);
        *(u8 *)(Func_02001a44(16) + 91) = 1;
        Func_02000e04(16);
        v5 = 0;
        *(u8 *)(Func_02001a56(16) + 91) = v5;
    }
}
