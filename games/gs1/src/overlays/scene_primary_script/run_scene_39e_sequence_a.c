#include "types.h"

#define FieldScene_RunScene39eSequenceA Func_020026d8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02006a34();
void Func_02006a92();
void Func_02006ab8();
void Func_02006adc();
void Func_02006b06();
void Func_02006b30();
s32 Func_02006b36();
void Func_02006b5a();
void Func_02006b66();
void Func_02006b68();
void Func_02006bb8();
void Func_02006c30();
void Func_02006c4c();
void Func_02006c76();
void Func_02006c86();
void Func_0200a5b9();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

void FieldScene_RunScene39eSequenceA(void)
{
    u32 i;
    s32 record;
    s32 base5_200a5b9;

    Func_02006ab8();
    base5_200a5b9 = (s32)Func_0200a5b9;
    Call2(Func_02006a34, base5_200a5b9, 0xc80);
    Call3(Func_02006b06, 0, 0x3333, 0x1999);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c8)) = 60;
    Func_02006c30();
    Func_02006c86(154);
    Func_02006b66(0, 2);
    Call3(Func_02006b5a, 0, 0, -6);
    Func_02006b68(0);
    Func_02006bb8(0, 15);
    record = Func_02006b36(0);
    Func_02006adc(record, 0);
    Func_02006a92(base5_200a5b9);
    Func_02006c76();
    Func_02006c4c(3);
    Func_02006b30();
}
