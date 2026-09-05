#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0000255e[];
extern u8 Data_03001ebc[];
void Func_02002ee8();
void Func_02002f32();
s32 Func_02002f3a();
void Func_02002f50();
void Func_02002f56();
void Func_02002f6e();
void Func_02002f76();
void Func_02002f7e();
void Func_02002f82();
void Func_02002f96();
void Func_02002f9a();
s32 Func_02002faa();
void Func_02002fbc();

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

s32 FieldScene_RunScene3ad_02001394(void)
{
    s32 record;
    s32 base5_255e;

    Func_02002f32(2, 0, 0);
    Call3(Func_02002f76, 2, 0x102, 60);
    base5_255e = (s32)Data_0000255e;
    Func_02002f56(base5_255e);
    Func_02002f6e(2, 0);
    Call3(Func_02002f82, 12, 0x3000, 0);
    Func_02002ee8(30);
    Func_02002f50(12, 4);
    Func_02002f7e((base5_255e + 1));
    Func_02002f96(12, 0);
    Call3(Func_02002fbc, 3, 0x102, 60);
    Func_02002f9a((base5_255e + 2));
    Value2(Func_02002faa, 3, 0);
    record = Value2(Func_02002f3a, 0, 0);
    return !record;
}
