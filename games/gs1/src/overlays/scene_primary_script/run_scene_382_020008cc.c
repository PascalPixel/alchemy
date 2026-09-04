#include "types.h"

#define FieldScene_RunScene382_020008cc Func_020008cc

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_000012c0[];
extern u8 Data_03001ebc[];
void Func_0200201c();
void Func_02002288();
void Func_020022a0();
void Func_020022bc();
void Func_02002326();
s32 Func_02002350();
void Func_02002362();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
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

void FieldScene_RunScene382_020008cc(void)
{
    u32 i;
    s32 record;

    Func_02002288();
    Func_02002326((s32)Data_000012c0);
    Func_0200201c(21, 0, 2);
    Call3(Func_02002362, 21, 0x103, 0);
    Func_020022a0(30);
    Value2(Func_02002350, 21, 0);
    Func_020022bc();
}
