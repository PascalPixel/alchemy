#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_0200b9e2();
s32 Func_0200b9ee();
s32 Func_0200ba0e();
void Func_0200ba14();
void Func_0200ba2c();
s32 Func_0200ba42();
s32 Func_0200ba60();

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

void FieldScene_RunScene373SequenceB(void)
{
    u32 i;
    s32 rec7;
    s32 record;

    if (Value1(Func_0200b9e2, 0x241) != 0) {
        rec7 = Value1(Func_0200b9ee, 0x106);
        if (rec7 != 0) {
            goto L_02005a8a;
        }
        *(u8 *)(Func_0200ba42(22) + 91) = rec7;
        Call1(Func_0200ba14, 0x241);
    } else {
        if (Value1(Func_0200ba0e, 0x106) != 0) {
            *(u8 *)(Func_0200ba60(22) + 91) = 1;
            Call1(Func_0200ba2c, 0x241);
        }
    }
    L_02005a8a:;
}
