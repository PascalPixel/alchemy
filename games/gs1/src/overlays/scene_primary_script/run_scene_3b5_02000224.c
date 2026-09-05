#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02000f66();
void Func_02000f7e();
s32 Func_02000fa6();
u8 *Func_02000fb8();

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3b5_02000224(void)
{
    u32 i;
    u8 *record;

    record = Value1(Func_02000fa6, 8);
    if ((s32)record != 0) {
        record[89] = 0;
    }
    record = Func_02000fb8(8);
    Func_02000f66((s32)record, 0);
    Call4(Func_02000f7e, 0, 0x2200000, 0x1200000, 253);
    Call1(Func_02000fa6, 0x200);
}
