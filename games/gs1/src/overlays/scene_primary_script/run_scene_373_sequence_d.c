#include "types.h"

#define FieldScene_RunScene373SequenceD Func_020011d8

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000f4d[];
extern u8 Data_03001ebc[];
s32 Func_0200717a();
void Func_02007196();
void Func_020071a2();
void Func_020071d4();
void Func_020071e0();
void Func_020071f2();
void Func_0200724e();
void Func_02007280();
void Func_020072a2();
void Func_020072ae();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
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

void FieldScene_RunScene373SequenceD(void)
{
    u32 i;
    s32 record;
    s32 base5_f4d;

    if (Value1(Func_0200717a, 0x808) == 0) {
        Func_020071a2();
        Call3(Func_020071e0, 0, 0x10000, 0x8000);
        base5_f4d = (s32)Data_00000f4d;
        Func_02007280(base5_f4d);
        Func_020072a2(15, 0, 2);
        Func_020072ae(16, 0, 2);
        Func_02007196((base5_f4d + 2), 1);
        Func_020071d4(6);
        Call3(Func_0200724e, 0, 69, 0x366);
        Func_020071f2();
    }
}
