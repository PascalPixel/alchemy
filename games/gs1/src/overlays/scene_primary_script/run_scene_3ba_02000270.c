#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02003e58();
void Func_02003eb0();
void Func_02003ed2();
void Func_02003ede();
void Func_02003ee0();
void Func_02003f12();
void Func_02003f2a();
s32 Func_02003f3c();
s32 Func_02003f96();
void Func_02003f9e();
void Func_02003fc6();
void Func_02004006();
void Func_02004020();
void Func_02004034();

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

void FieldScene_RunScene3ba_02000270(void)
{
    u32 i;
    u8 *rec7;
    s32 record;
    s32 none;

    Call1(Func_02003ede, 0x301);
    rec7 = Value1(Func_02003f3c, 13);
    Func_02003f2a();
    Call2(Func_02004006, 0x20000, 0x4000);
    Call4(Func_02004020, 0x2580000, -1, 0xc80000, 1);
    Func_02003e58((s32)rec7, 3);
    Func_02004034();
    none = 0;
    rec7[85] = none;
    *(s32 *)((s32)rec7 + 52) = 0x6666;
    *(s32 *)((s32)rec7 + 48) = 0xcccc;
    Call4(Func_02003eb0, (s32)rec7, *(s32 *)((s32)rec7 + 8), 0x80000, *(s32 *)((s32)rec7 + 16));
    rec7 = Value1(Func_02003f96, 14);
    rec7[85] = none;
    *(s32 *)((s32)rec7 + 52) = 0x6666;
    *(s32 *)((s32)rec7 + 48) = 0xcccc;
    Call4(Func_02003ed2, (s32)rec7, *(s32 *)((s32)rec7 + 8), 0x200000, *(s32 *)((s32)rec7 + 16));
    Func_02003ee0((s32)rec7);
    Func_02003f9e(45);
    Call6(Func_02003f12, 43, 12, 1, 1, 41, 12);
    Func_02003fc6();
}
