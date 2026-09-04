#include "types.h"

#define FieldScene_RunScene39f_02002004 Func_02002004

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02002dc4();
void Func_02002e52();
void Func_02002eb8();
void Func_02004daa();
void Func_02004dac();
void Func_02004dee();
void Func_02004df0();
s32 Func_02004e04();
void Func_02004e18();
void Func_02004e5c();
void Func_02004e66();
void Func_02004e86();

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

void FieldScene_RunScene39f_02002004(void)
{
    u32 i;
    s32 record;

    Func_02004dac();
    Func_02002eb8(18, 1);
    Call4(Func_02004e86, 0x2e80000, -1, 0x1f80000, 1);
    Call4(Func_02002dc4, 18, 0x2e8, 0x1f8, 0x90000);
    Func_02002e52(18);
    Func_02004e66(18, 15);
    record = Func_02004e04(18);
    Func_02004daa(record, 0);
    Func_02004df0(30);
    Call1(Func_02004dee, 0x30a);
    Call3(Func_02004e5c, 22, 0x2e80000, 0x1f80000);
    Func_02004e18();
}
