#include "types.h"

#define FieldScene_RunScene3b1_02003dec Func_02003dec

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020086e2();
void Func_02008710();
void Func_0200a2b8();
void Func_0200a386();
void Func_0200a3bc();
void Func_0200a3d6();
void Func_0200a3de();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3b1_02003dec(void)
{
    u32 i;
    s32 record;

    Func_0200a2b8();
    Func_020086e2(15, 1, 1);
    Call3(Func_0200a3d6, 8, 0x5000, 40);
    Func_0200a386(8, 2);
    Call1(Func_0200a3bc, 0x1e3d);
    Func_0200a3de(8, 0, 20);
    Func_02008710(9, 11, 0);
}
