#include "types.h"

#define FieldScene_RunScene371_0200281c Func_0200281c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200e79c[];
extern u8 Data_03001ebc[];
void Func_02006afc();
void Func_02006b2e();
void Func_02006bb6();
void Func_02006bd4();
s32 Func_02006bee();
void Func_02006c0a();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

void FieldScene_RunScene371_0200281c(void)
{
    u32 i;
    s32 record;

    Func_02006afc();
    Func_02006bb6(55, 0, 0);
    Call1(Func_02006bd4, 0x2642);
    Value2(Func_02006bee, *(volatile s32 *)Data_0200e79c, 0);
    Call3(Func_02006c0a, 55, 0x3000, 0);
    Func_02006b2e();
}
