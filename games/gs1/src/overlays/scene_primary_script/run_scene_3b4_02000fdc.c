#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_0200348e();
void Func_020034e6();
void Func_020034f0();
void Func_020034f4();
void Func_0200350a();
void Func_02003526();
void Func_02003538();
void Func_020035ee();

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

void FieldScene_RunScene3b4_02000fdc(s32 a0)
{
    u32 i;
    s32 record;

    if ((a0 & 0x100) != 0) {
        Func_020035ee(157);
        Call3(Func_020034e6, 0x20000, 0x20000, 0x10000);
        Call3(Func_020034f4, -1, -1, 0xe666);
        Call6(Func_020034f0, 84, 29, 1, 3, 70, 49);
        Func_0200348e(60);
    }
    Call6(Func_0200350a, 85, 29, 1, 3, 70, 49);
    Call6(Func_02003526, 6, 49, 1, 1, 6, 50);
    Call6(Func_02003538, 6, 49, 1, 1, 6, 51);
}
