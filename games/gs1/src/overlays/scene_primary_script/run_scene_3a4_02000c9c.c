#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001122();
void Func_020047de();
void Func_02004812();
void Func_02004866();
s32 Func_0200486e();
void Func_02004870();
void Func_02004892();
void Func_020048b6();
void Func_020048c4();
void Func_020048d8();
void Func_020048ea();
void Func_02004902();
void Func_020049dc();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
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

void FieldScene_RunScene3a4_02000c9c(void)
{
    u32 i;
    s32 record;

    Func_02004812();
    Call6(Func_020047de, 24, 27, 2, 1, 24, 26);
    Func_020049dc(185);
    Call3(Func_02004866, 10, 0x3333, 0x1999);
    Call3(Func_02004870, 0, 0x3333, 0x1999);
    *(u8 *)(Func_0200486e(10) + 90) &= 254;
    Func_020048d8(0, 8);
    Call3(Func_020048b6, 0, 0x190, 0x1a8);
    Call3(Func_020048c4, 10, 0x198, 0x1a8);
    Func_020048ea(10);
    Func_02004902(0, 1);
    Func_02001122();
    Func_02004892();
}
