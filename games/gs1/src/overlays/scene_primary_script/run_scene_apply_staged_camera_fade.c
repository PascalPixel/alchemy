#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_020033fe();
void Func_02003458();
void Func_02003460();
void Func_02003466();
void Func_02003478();
void Func_02003492();
void Func_020034a4();
void Func_02003560();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void RunScene_ApplyStagedCameraFade(s32 a0)
{
    s32 step77 = ((255 & a0) << 2) + 77;
    s32 step13 = ((255 & a0) << 2) + 13;

    if ((0x100 & a0) != 0) {
        Func_02003560(157);
        Call3(Func_02003458, 0x20000, 0x20000, 0x10000);
        Call3(Func_02003466, -1, -1, 0xe666);
        Call6(Func_02003460, 79, 29, 1, 3, step77, 40);
        Func_020033fe(40);
    }
    Call6(Func_02003478, 80, 29, 1, 3, step77, 40);
    Call6(Func_02003492, step13, 40, 1, 1, step13, 41);
    Call6(Func_020034a4, step13, 40, 1, 1, step13, 42);
}
