#include "types.h"

#define FieldScene_RunScene3b9_0200055c Func_0200055c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020021e8();
void Func_020021ee();
void Func_02002200();
void Func_02002210();
void Func_0200222e();
s32 Func_0200313a();
void Func_02003154();
void Func_02003174();
void Func_0200319c();
void Func_020031d8();
void Func_020031e4();
void Func_02003202();
void Func_0200320e();
void Func_02003216();
void Func_02003224();
void Func_0200322e();
void Func_02003286();

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

void FieldScene_RunScene3b9_0200055c(void)
{
    u32 i;
    s32 record;

    Func_02003154();
    Func_020031e4(16, 2);
    Call1(Func_02003202, 0x211b);
    Func_02003224(16, 0, 20);
    if (Value1(Func_0200313a, 0x3c1) != 0) {
        Func_02003174(20);
    } else {
        Func_020021ee(17, 0);
        Func_02003216(17, 1);
        Func_020021e8(17);
        Func_0200322e(17, 0, 20);
        Func_0200320e(17, 4);
        Func_02002200(17);
        Call3(Func_02003286, 17, 0x105, 40);
        Func_02002210(17);
        Call2(Func_0200222e, 17, 0x5000);
        Call1(Func_0200319c, 0x3c1);
    }
    Func_020031d8();
}
