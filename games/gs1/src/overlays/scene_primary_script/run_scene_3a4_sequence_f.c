#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_0200364c();
void Func_0200382e();
void Func_020068c4();
void Func_02006936();
void Func_02006990();
void Func_02006996();
void Func_020069e8();
void Func_02006a12();
void Func_02006a20();
void Func_02006b0c();
void Func_02006b18();

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

void FieldScene_RunScene3a4SequenceF(void)
{
    u32 i;
    s32 record;
    s32 v6;

    Func_02006990();
    Func_02006a12(9, 0, 0);
    Call3(Func_02006a20, 8, 0x1480000, 0x1a80000);
    Call1(Func_02006996, 0x323);
    Func_0200382e();
    Func_02006936();
    v6 = 224;
    Func_020068c4(1);
    *(volatile s32 *)(*(volatile s32 *)Data_03001ebc + (v6 << 1)) = 0x201;
    Func_02006b0c();
    Func_02006b18();
    Func_0200364c();
    *(volatile s32 *)(*(volatile s32 *)Data_03001ebc + (v6 << 1)) = 0x204;
    Func_020069e8();
}
