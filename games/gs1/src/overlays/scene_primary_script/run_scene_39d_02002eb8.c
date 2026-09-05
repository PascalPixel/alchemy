#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020060fc();
void Func_02006594();
void Func_020065c2();
void Func_020065ec();
void Func_020065f0();
void Func_02006668();
void Func_02006678();
void Func_02006680();
void Func_020066b6();
void Func_020066d4();

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

void FieldScene_RunScene39d_02002eb8(void)
{
    u32 i;
    s32 record;

    Func_02006594();
    Call3(Func_020065c2, 0, 0x8000, 0x4000);
    Func_020065ec(0, 104, 152);
    Call3(Func_02006668, 0, 0x4000, 60);
    Func_02006678(17, 0);
    Func_02006680(18, 0);
    Func_020060fc();
    Call4(Func_020066b6, -1, -1, -1, 0);
    Func_020066d4(1);
    Func_020065f0();
}
