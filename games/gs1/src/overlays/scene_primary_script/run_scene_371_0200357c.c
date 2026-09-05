#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_0200772e();
s32 Func_02007778();
void Func_02007802();
void Func_0200780e();
void Func_0200785e();
void Func_0200786a();
s32 Func_0200787e();
s32 Func_020078b4();
s32 Func_020078c0();
void Func_020078e0();
void Func_020078ea();
s32 Func_020078f6();
void Func_020078fc();
void Func_02007946();
void Func_02007988();
void Func_02007a22();
void Func_02007a26();
void Func_02007a4c();
void Func_02007a50();
void Func_02007a7e();
void Func_02007a8a();
void Func_02007aac();

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

void FieldScene_RunScene371_0200357c(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_0200787e, 8);
    Func_0200785e(60);
    Func_0200786a();
    Call2(Func_02007a22, 0x9999, 1);
    *(s32 *)(rec7 + 24) = 0x13333;
    *(s32 *)(rec7 + 28) = 0x13333;
    Func_02007988(8, 1);
    Func_0200772e(1);
    Func_02007946(0, 15);
    record = Func_020078b4(0);
    Func_02007802(record, 0);
    record = Func_020078c0(8);
    Func_0200780e(record, 0);
    Call3(Func_020078e0, 8, 0x6666, 0x3333);
    {
        s32 shown = 0;

        *(u16 *)(rec7 + 100) = shown;
    }
    Value2(Func_020078f6, 8, 0x200d218);
    Value2(Func_02007778, 0x200b4a1, 0xc80);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x100;
    Call2(Func_02007a26, 0x10003, 1);
    *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c8) = 32;
    Func_02007a4c();
    Func_020078ea(120);
    Call2(Func_02007aac, 0x16666, 0x12c);
    Call1(Func_020078fc, 0x10e);
    *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c8) = 16;
    *(u16 *)0x05000000 = 0x7fff;
    Func_02007a7e();
    Func_02007a8a();
    Func_02007a50(111);
}
