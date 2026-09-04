#include "types.h"

#define FieldScene_RunScene39b_0200196c Func_0200196c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02003e30();
void Func_02003e44();
void Func_02003e60();
s32 Func_02003e76();
s32 Func_02003e80();
s32 Func_02003e88();
void Func_02003ea6();
void Func_02003ebe();
void Func_02003ece();
void Func_02003ed8();
void Func_02003edc();
void Func_02003f00();
void Func_02003f08();
void Func_02003f1c();
void Func_02003f20();
void Func_02003f28();
void Func_02003f2c();
void Func_02003f3e();
void Func_02003f46();
void Func_02003f9c();
void Func_02003fb0();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene39b_0200196c(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02003e30, 0x250) == 0) {
        Call1(Func_02003e44, 0x250);
        Func_02003e60();
        record = Func_02003e76(12);
        *(volatile s32 *)(record + 24) = -0x10000;
        record = Value1(Func_02003e80, 13);
        *(volatile s32 *)(record + 24) = -0x10000;
        record = Func_02003e88(14);
        *(volatile s32 *)(record + 24) = -0x10000;
        Call3(Func_02003ed8, 3, 0x880000, 0x900000);
        Call3(Func_02003f1c, 3, 0x4000, 10);
        *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0) = 0x201;
        Func_02003f9c();
        Func_02003fb0();
        Func_02003ea6(60);
        Func_02003f28(3, 0, 0);
        Func_02003f20(3, 3);
        Func_02003ebe(30);
        Func_02003f00(3, 136, 72);
        Func_02003ece(40);
        Func_02003f46(0, 1);
        Func_02003f2c(3);
        Func_02003f3e(3, 0, 0);
        Call1(Func_02003edc, 0x872);
        *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0) = 0x204;
        Func_02003f08();
    }
}
