#include "types.h"

#define FieldScene_RunScene3b5_02000528 Func_02000528

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00001fa0[];
extern u8 Data_03001ebc[];
void Func_02001288();
void Func_020012c2();
void Func_02001326();
void Func_0200134a();
void Func_0200135a();
void Func_0200135e();
void Func_0200136e();

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

void FieldScene_RunScene3b5_02000528(void)
{
    u32 i;
    s32 record;

    Func_02001288();
    Func_02001326((s32)Data_00001fa0);
    Call3(Func_0200135a, 25, 0xc000, 0);
    Func_0200134a(25, 0);
    Call3(Func_0200136e, 25, 0x8000, 0);
    Func_0200135e(25, 0);
    Func_020012c2();
}
