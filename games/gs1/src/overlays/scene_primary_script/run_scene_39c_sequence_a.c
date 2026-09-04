#include "types.h"

#define FieldScene_RunScene39cSequenceA Func_02001d3c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02007790();
void Func_02007798();
s32 Func_020077c8();
void Func_020077e6();
void Func_02007800();
void Func_02007826();
s32 Func_0200782c();
s32 Func_0200783c();
s32 Func_02007844();
void Func_0200785c();
void Func_020079f6();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene39cSequenceA(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    if (Value1(Func_020077c8, 0x256) != 0) {
        Func_02007800();
        Call1(Func_020077e6, 0x256);
        record = Func_0200782c(0);
        *(volatile s32 *)(record + 12) += 0x20000;
        rec7 = Value1(Func_0200783c, 0);
        record = Value1(Func_02007844, 0);
        *(volatile s32 *)(rec7 + 60) = *(volatile s32 *)(record + 12);
        Func_02007826(5);
        Call6(Func_02007790, 8, 29, 10, 23, 1, 1);
        Func_020079f6(217);
        Call3(Func_02007798, 0x200e08e, 10, 18);
        Func_0200785c();
    }
}
