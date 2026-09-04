#include "types.h"

#define FieldScene_RunScene38a_02000380 Func_02000380

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02000892();
void Func_02000aa6();
s32 Func_02000aac();
void Func_02000aba();
u8 *Func_02000ac8();
void Func_02000ae8();
u8 *Func_02000aec();

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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void FieldScene_RunScene38a_02000380(void)
{
    s32 *record;
    u8 *target;
    s32 value;

    record = (s32 *)Value1(Func_02000aac, 8);
    value = record[2] / 0x100000;
    if (value == 24) {
        Func_02000892(8);
        {
            u8 *record = Func_02000ac8(8);
            u8 value = *(volatile u8 *)&record[35];

            record[35] = (u8)(value | 2);
        }
        Call6(Func_02000aa6, 19, 74, 9, 3, 19, 17);
        target = Func_02000aec(8);
        Func_02000aba((s32)target, 0);
        Call1(Func_02000ae8, 0x864);
    }
}
