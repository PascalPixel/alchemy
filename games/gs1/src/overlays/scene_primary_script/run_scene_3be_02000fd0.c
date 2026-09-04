#include "types.h"

#define FieldScene_RunScene3be_02000fd0 Func_02000fd0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_000023cc[];
extern u8 Data_03001ebc[];
s32 Func_020024f0();
s32 Func_020024fa();
s32 Func_02002514();
void Func_02002564();
s32 Func_02002574();
void Func_02002596();
void Func_020025ae();
void Func_020025c8();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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

void FieldScene_RunScene3be_02000fd0(void)
{
    u32 i;
    s32 record;
    s32 base5_23cc;

    base5_23cc = (s32)Data_000023cc;
    Func_02002564(base5_23cc);
    Value2(Func_02002574, 8, 0);
    if (Value2(Func_02002514, 0, 0) == 0) {
        if (Value1(Func_020024f0, 0x950) != 0) {
            if (Value1(Func_020024fa, 0x96f) == 0) {
                Func_02002596((base5_23cc + 8));
            }
        }
        Func_020025ae(8, 0);
    } else {
        bump_step(1);
        Func_020025c8(8, 0);
    }
}
