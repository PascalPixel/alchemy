#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02003d64();
void Func_02003d8e();
void Func_02003d98();
s32 Func_02003d9c();
void Func_02003dbe();
void Func_02003dca();
void Func_02003dda();
void Func_02003de4();
void Func_02003de6();
void Func_02003df0();
void Func_02003df6();
void Func_02003e04();
void Func_02003e16();
void Func_02003e56();
void Func_02003e84();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3b4SequenceB(void)
{
    u32 i;
    s32 record;

    Func_02003d64();
    Call3(Func_02003d8e, 0, 0x1b333, 0xd999);
    Call3(Func_02003d98, 12, 0x1b333, 0xd999);
    Func_02003e56(188);
    record = Value1(Func_02003d9c, 0);
    if (record != 0) {
        Func_02003dbe(12, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02003de4(12);
    Func_02003de6(0, 0, 24);
    Func_02003e84(188);
    Func_02003df6(12, 0, 16);
    Func_02003e04(0);
    Call3(Func_02003df0, 12, 0x138, 232);
    Func_02003e16(12);
    Func_02003dda();
    Call1(Func_02003dca, 0x220);
}
