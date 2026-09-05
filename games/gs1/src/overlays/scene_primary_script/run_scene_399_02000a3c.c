#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_02002c7e();
s32 Func_02002c8a();
void Func_02002c8e();
void Func_02002cd8();
void Func_02002d48();
void Func_02002d78();
void Func_02002d88();
void Func_02002d98();
void Func_02002dbc();
void Func_02002e38();

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

void FieldScene_RunScene399_02000a3c(void)
{
    u32 i;
    s32 record;

    record = Func_02002c8a(0);
    if ((u32)((*(volatile u16 *)(record + 6) + 0x5fff) << 16) <= 0x3ffe0000) {
        Func_02002e38(4, 16);
    } else {
        Func_02002c8e();
        Func_02002d48(16, 0, 10);
        if (Value1(Func_02002c7e, 0x881) != 0) {
            Call1(Func_02002d78, 0x1653);
            Func_02002d98(16, 0);
        } else {
            Call1(Func_02002d88, 0x154b);
            Func_02002d98(16, 0);
        }
        Call3(Func_02002dbc, 16, 0x3000, 10);
        Func_02002cd8();
    }
}
