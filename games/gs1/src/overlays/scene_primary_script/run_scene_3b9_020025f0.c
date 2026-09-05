#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020051e8();
s32 Func_020051fe();
void Func_02005212();
void Func_02005254();
void Func_02005262();
void Func_02005266();
void Func_02005268();
void Func_0200531e();
void Func_02005348();
void Func_02005350();
void Func_0200535c();
void Func_02005368();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

void FieldScene_RunScene3b9_020025f0(void)
{
    u32 i;
    s32 record;

    Func_020051e8();
    Call3(Func_02005212, 0, 0x19999, 0xcccc);
    Func_0200531e();
    Func_02005266(0, 2);
    Call3(Func_02005254, 0, 0x30c, 0x1ac);
    Call3(Func_02005262, 0, 0x370, 0x1ac);
    Call3(Func_02005268, 0, 0x3d4, 0x1ac);
    Func_0200535c();
    Func_02005368();
    if (Value1(Func_020051fe, 0x90f) != 0) {
        Func_02005348(32);
    } else {
        Func_02005350(12);
    }
}
