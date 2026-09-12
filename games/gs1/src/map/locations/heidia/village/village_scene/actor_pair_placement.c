#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02007cd6();
void Func_02007d22();
void Func_02007d30();
void Func_02007d34();
void Func_02007d64();
void Func_02007d70();
void Func_02007d96();
void Func_02007d9c();
void Func_02007dae();
void Func_02007db4();
void Func_02007de8();
void Func_02007dec();
void Func_02007e20();

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

void FieldScene_RunScene3bf_0200269c(void)
{
    u32 i;
    s32 record;

    Func_02007dec(11, 1);
    Func_02007de8();
    Func_02007cd6(60);
    Call1(Func_02007d9c, 0x247c);
    Func_02007db4(13, 0);
    Call3(Func_02007d22, 11, 0x10000, 0x8000);
    Call3(Func_02007d30, 15, 0x10000, 0x8000);
    Call3(Func_02007d64, 11, 0x1d8, 180);
    Call3(Func_02007d70, 15, 0x1d8, 180);
    Func_02007e20(11, 1);
    Func_02007d96(11);
    Func_02007dae(11, 4);
    Func_02007d34(30);
}
