#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020037a0();
void Func_0200393e();
void Func_0200692c();
void Func_0200699e();
void Func_020069f8();
void Func_020069fe();
void Func_02006a50();
void Func_02006a7a();
void Func_02006a88();
void Func_02006b74();
void Func_02006b80();

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

void FieldScene_RunScene3a4SequenceE(void)
{
    u32 i;
    s32 record;
    s32 v6;

    Func_020069f8();
    Func_02006a7a(10, 0, 0);
    Call3(Func_02006a88, 8, 0x1e80000, 0x8a0000);
    Call1(Func_020069fe, 0x325);
    Func_0200393e();
    Func_0200699e();
    v6 = 224;
    Func_0200692c(1);
    *(volatile s32 *)(*(volatile s32 *)Data_03001ebc + (v6 << 1)) = 0x201;
    Func_02006b74();
    Func_02006b80();
    Func_020037a0();
    *(volatile s32 *)(*(volatile s32 *)Data_03001ebc + (v6 << 1)) = 0x204;
    Func_02006a50();
}
