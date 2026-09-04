#include "types.h"

#define FieldScene_RunScene3b9_02000334 Func_02000334

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001f9e();
void Func_02001fd2();
s32 Func_02002efa();
void Func_02002f2c();
void Func_02002f52();
void Func_02002f86();
void Func_02002fc6();
void Func_02002fe4();
void Func_02003012();
s32 Func_02003022();
void Func_0200302a();

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3b9_02000334(void)
{
    u32 i;
    s32 record;

    Func_02002f2c();
    if (Value1(Func_02002efa, 0x962) != 0) {
        Func_02002fc6(14, 2);
        Call1(Func_02002fe4, 0x2256);
        Func_02001f9e(14);
        Func_02002fe4(14, 0, 0);
        Func_02002f52(20);
        Value2(Func_02003022, 14, 0);
        Func_02001fd2(14, 0);
    } else {
        Call1(Func_02003012, 0x205d);
        Func_0200302a(14, 0);
    }
    Func_02002f86();
}
