#include "types.h"

#define RunEventScript02 Func_020019bc

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02003bf0();
void Func_02003c16();
void Func_02003c20();
void Func_02003c5a();
void Func_02003d44();

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
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void RunEventScript02(void)
{
    u32 i;
    u8 *record;

    Func_02003bf0();
    Func_02003d44();
    Call3(Func_02003c20, 0, 0x20000, 0x1999);
    Func_02003c5a(0, 232, 204);
    Func_02003c16();
}
