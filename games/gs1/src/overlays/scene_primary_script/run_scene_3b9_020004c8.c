#include "types.h"

#define FieldScene_RunScene3b9_020004c8 Func_020004c8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02002132();
void Func_020030c0();
void Func_020030fe();
void Func_0200315a();
void Func_02003178();
void Func_020031a2();
void Func_020031ba();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

void FieldScene_RunScene3b9_020004c8(void)
{
    u32 i;
    s32 record;

    Func_020030c0();
    Call2(Func_020031a2, 14, 0x102);
    Func_0200315a(14, 2);
    Call1(Func_02003178, 0x2116);
    Func_02002132(14);
    Call3(Func_020031ba, 14, 0x102, 40);
    Func_020031a2(14, 0);
    Func_020030fe();
}
