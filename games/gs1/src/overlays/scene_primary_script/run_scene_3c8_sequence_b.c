#include "types.h"

#define FieldScene_RunScene3c8SequenceB Func_02002f30

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
s32 Func_02002fc4();
void Func_02007dc8();
void Func_02007df6();
void Func_02007e08();
void Func_02007e22();
void Func_02007e2c();
void Func_02007e92();
void Func_02007eb8();
void Func_02007f26();

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

void FieldScene_RunScene3c8SequenceB(void)
{
    u32 i;
    s32 record;

    Func_02007dc8();
    Call3(Func_02007df6, 0, 0x8000, 0x4000);
    Call3(Func_02007e2c, 0, 0x208, 0x2c8);
    Call3(Func_02007eb8, 0, 0x4000, 10);
    (void)Func_02002fc4(0x2080000, 0, 0x3100000, 223);
    Func_02007e92(0, 6, 0);
    Func_02007e08(60);
    Func_02007f26(20);
    Func_02007e22();
}
