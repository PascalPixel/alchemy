#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020059a4();
void Func_02005ae4();
void Func_02005af4();
s32 Func_02005afe();
void Func_02005b4e();
void Func_02005b54();
void Func_02005b86();
void Func_02005b96();
void Func_02005c0e();
void Func_02005c1e();
void Func_02005c72();
void Func_02005c7a();
void Func_02005c9a();
void Func_02005ca6();

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene371_020017fc(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    rec7 = Value1(Func_02005afe, 8);
    Func_02005ae4();
    Call4(Func_02005c0e, -1, -1, -1, 0);
    Func_020059a4(1);
    Func_02005b86(0, 0, 0);
    *(s32 *)(rec7 + 28) = 0x14000;
    *(s32 *)(rec7 + 24) = 0x14000;
    Func_02005c1e(8, 1);
    Func_02005c7a();
    Call3(Func_02005b54, 8, 0x6666, 0x3333);
    Call3(Func_02005b96, 8, 0x14a8, 0x918);
    Func_02005c9a();
    Func_02005ca6();
    Call1(Func_02005af4, 0x927);
    Func_02005c72(102);
    Func_02005b4e();
}
