#include "types.h"

#define FieldScene_RunScene3bfSequenceB Func_02000d54

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_02006346();
void Func_02006358();
void Func_02006376();
void Func_02006390();

extern s32 Data_0200f73c[];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void FieldScene_RunScene3bfSequenceB(s32 a0)
{
    s32 position_x;
    s32 position_z;

    position_x = Data_0200f73c[a0 * 2];
    position_z = Data_0200f73c[a0 * 2 + 1];
    Call6(Func_02006346, 0, 77, 1, 3, position_x, position_z);
    Call6(Func_02006358, 1, 77, 1, 1, position_x + 1, position_z);
    Call6(Func_02006376, position_x, position_z - 45, 1, 1, position_x, position_z - 44);
    if (a0 == 1) {
        Call6(Func_02006390, position_x, position_z - 44, 1, 1, position_x, position_z - 43);
    }
}
