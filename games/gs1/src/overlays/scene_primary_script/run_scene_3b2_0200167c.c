#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020046c4();
s32 Func_020046dc();
s32 Func_02004714();
s32 Func_02004738();
s32 Func_0200473a();
s32 Func_0200475c();
s32 Func_0200475e();
void Func_02004762();
s32 Func_02004782();
void Func_02004786();
s32 Func_020047a6();
void Func_020047aa();
void Func_020047ce();

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

void FieldScene_RunScene3b2_0200167c(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_020046dc, 0x8c4) != 0) {
        Call6(Func_020046c4, 0, 0, 1, 1, 8, 21);
        Call3(Func_02004762, 15, 0x3c80000, 0x3c80000);
    } else {
        record = Func_0200473a(15);
        *(volatile s32 *)(record + 28) = 0x19999;
    }
    if (Value1(Func_02004714, 0x8c5) != 0) {
        Call3(Func_02004786, 16, 0x3c80000, 0x3c80000);
    } else {
        record = Func_0200475e(16);
        *(volatile s32 *)(record + 28) = 0x19999;
    }
    if (Value1(Func_02004738, 0x8c6) != 0) {
        Call3(Func_020047aa, 17, 0x3c80000, 0x3c80000);
    } else {
        record = Func_02004782(17);
        *(volatile s32 *)(record + 28) = 0x19999;
    }
    if (Value1(Func_0200475c, 0x8c7) != 0) {
        Call3(Func_020047ce, 18, 0x3c80000, 0x3c80000);
    } else {
        record = Func_020047a6(18);
        *(volatile s32 *)(record + 28) = 0x19999;
    }
}
