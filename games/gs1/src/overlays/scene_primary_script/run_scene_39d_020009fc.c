#include "types.h"

#define FieldScene_RunScene39d_020009fc Func_020009fc

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200bc50[];
extern u8 Data_03001ebc[];
s32 Func_020012d8();
void Func_02004082();
void Func_0200409c();
void Func_020040b8();
void Func_020040d0();
void Func_020040d4();
void Func_020040e0();
s32 Func_020040f2();
s32 Func_02004112();
void Func_0200419a();

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene39d_020009fc(void)
{
    u32 i;
    u8 *rec;
    u8 *rec8;
    s32 record;
    s32 base5_200bc50;

    rec = Value1(Func_020040f2, 0);
    Func_020040e0();
    record = Func_020012d8();
    base5_200bc50 = (s32)Data_0200bc50;
    *(s32 *)base5_200bc50 = record;
    if (record != 0) {
        Call1(Func_020040d4, 0x250);
        rec8 = Value1(Func_02004112, *(s32 *)base5_200bc50);
        rec8[85] = 0;
        rec[85] &= 254;
        *(s32 *)((s32)rec8 + 12) += -0x30000;
        *(s32 *)((s32)rec + 12) += -0x30000;
        *(s32 *)((s32)rec + 20) += -0x30000;
        Func_02004082(2);
        *(s32 *)((s32)rec8 + 12) += -0x20000;
        *(s32 *)((s32)rec + 12) += -0x20000;
        *(s32 *)((s32)rec + 20) += -0x20000;
        Func_0200409c(10);
        *(s32 *)((s32)rec8 + 12) += 0x20000;
        *(s32 *)((s32)rec + 12) += 0x20000;
        *(s32 *)((s32)rec + 20) += 0x20000;
        Func_020040b8(4);
        *(s32 *)((s32)rec8 + 12) += 0x20000;
        *(s32 *)((s32)rec + 12) += 0x20000;
        *(s32 *)((s32)rec + 20) += 0x20000;
        Func_020040d0(4);
        *(s32 *)((s32)rec8 + 12) += 0x10000;
        *(s32 *)((s32)rec + 12) += 0x10000;
        *(s32 *)((s32)rec + 20) += 0x10000;
    }
    Func_0200419a();
}
