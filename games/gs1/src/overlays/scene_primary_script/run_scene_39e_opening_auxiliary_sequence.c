#include "types.h"

#define FieldScene_RunOpeningAuxiliarySequence Func_0200268c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02006a70();
void Func_02006ab4();
void Func_02006ab6();
s32 Func_02006ae8();
void Func_02006b08();
void Func_02006b12();
void Func_02006b1a();
void Func_02006b20();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunOpeningAuxiliarySequence(s32 a0, s32 a1)
{
    u32 i;
    s32 record;

    Func_02006a70();
    Call3(Func_02006ab6, 0, 0x28000, 0x14000);
    Value3(Func_02006ae8, 0, a0, a1);
    Func_02006b1a(0, 4, 0);
    Func_02006b12(0, 7);
    Func_02006b08(0);
    Func_02006b20(0, 6);
    Func_02006ab4();
}
