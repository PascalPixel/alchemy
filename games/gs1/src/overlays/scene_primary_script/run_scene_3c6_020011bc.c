#include "types.h"

#define FieldScene_RunScene3c6_020011bc Func_020011bc

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020026f0();
void Func_02002716();
void Func_0200278e();
void Func_020027a6();
void Func_02002802();

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

void FieldScene_RunScene3c6_020011bc(void)
{
    u32 i;
    s32 record;

    Func_020026f0();
    Call1(Func_0200278e, 0x28b7);
    Func_020027a6(1, 0);
    Call3(Func_02002802, 0, 0, -16);
    Func_02002716();
}
