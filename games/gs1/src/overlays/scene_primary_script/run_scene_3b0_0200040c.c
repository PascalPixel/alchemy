#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02000908();
void Func_020015dc();
void Func_020015fe();
void Func_02001620();
void Func_02001624();
void Func_02001678();
void Func_020016b8();
void Func_020016e6();
s32 Func_020016f8();
void Func_02001704();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

void FieldScene_RunScene3b0_0200040c(void)
{
    u32 i;
    s32 record;

    Func_02001624();
    Call4(Func_020016e6, -1, -1, -1, 0);
    Func_020015dc(1);
    *(u8 *)(Func_020016f8() + 85) = 0;
    Call3(Func_02001704, 0xa40000, 0x400000, 0x1410000);
    Func_02001620();
    Func_020015fe(1);
    Func_020016b8(0, 0, 0);
    Func_02000908();
    Func_02001678();
}
