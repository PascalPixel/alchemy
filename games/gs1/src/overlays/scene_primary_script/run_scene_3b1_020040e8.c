#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00001e40[];
extern u8 Data_03001ebc[];
void Func_020089de();
void Func_02008a1a();
void Func_0200a5b4();
void Func_0200a5c4();
void Func_0200a67e();
void Func_0200a690();
void Func_0200a6c6();
void Func_0200a6e0();
void Func_0200a6e8();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

void FieldScene_RunScene3b1_020040e8(void)
{
    u32 i;
    s32 record;

    Func_0200a5b4();
    Func_020089de(15, 1, 1);
    Func_0200a67e(8, 1);
    Func_0200a5c4(10);
    Call3(Func_0200a6e0, 8, 0x3000, 20);
    Func_0200a690(8, 2);
    Func_0200a6c6((s32)Data_00001e40);
    Func_0200a6e8(8, 0, 20);
    Func_02008a1a(9, 14, 0);
}
