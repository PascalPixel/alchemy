#include "types.h"

#define FieldScene_RunScene382_020012c0 Func_020012c0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02002c6e();
void Func_02002c80();
void Func_02002c82();
void Func_02002ca4();
void Func_02002cc0();
void Func_02002cca();
void Func_02002cd0();
void Func_02002ce0();
void Func_02002ce4();
void Func_02002cf2();
void Func_02002cf6();
void Func_02002d10();
void Func_02002d18();
void Func_02002d3a();
void Func_02002d44();
void Func_02002d4a();
void Func_02002d5a();
void Func_02002d70();
void Func_02002dbe();

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

void FieldScene_RunScene382_020012c0(void)
{
    u32 i;
    s32 record;

    Func_02002c6e(231);
    Func_02002c82();
    Func_02002c80(10);
    Func_02002d10(19, 2);
    Call3(Func_02002cca, 19, 0xcccc, 0x6666);
    Call3(Func_02002cf6, 19, 216, 0x198);
    Func_02002ca4(10);
    Call3(Func_02002d70, 19, 0x4000, 20);
    Func_02002d3a(19, 6, 0);
    Func_02002cc0(30);
    Func_02002d4a(19, 6, 0);
    Func_02002cd0(30);
    Func_02002d5a(19, 6, 0);
    Func_02002ce0(30);
    Call3(Func_02002d44, 19, 216, 0x188);
    Func_02002cf2(10);
    Call3(Func_02002dbe, 19, 0x4000, 20);
    Call1(Func_02002ce4, 0x858);
    Func_02002d18();
}
