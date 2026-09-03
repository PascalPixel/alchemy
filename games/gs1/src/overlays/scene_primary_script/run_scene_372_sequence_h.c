#include "types.h"

#define FieldScene_BuildPlacementGrid Func_020040f0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_020087a8();
void Func_020087be();
void Func_020087d0();
void Func_020087e4();
void Func_020087f6();
void Func_0200880a();
void Func_0200881c();
void Func_0200882c();
void Func_02008858();
void Func_0200886c();
void Func_02008880();
void Func_02008892();
void Func_020088a4();
void Func_020088a6();
void Func_020088b6();
void Func_020088ca();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_BuildPlacementGrid(void)
{
    u32 i;
    u8 *record;

    Call6(Func_020087a8, 16, 96, 11, 73, 6, 3);
    Call6(Func_020087be, 16, 96, 34, 68, 14, 10);
    Call6(Func_020087d0, 16, 96, 64, 68, 7, 7);
    Call6(Func_020087e4, 9, 95, 11, 73, 6, 3);
    Call6(Func_020087f6, 40, 94, 34, 68, 14, 10);
    Call6(Func_0200880a, 54, 94, 64, 68, 8, 7);
    Call6(Func_0200881c, 72, 75, 72, 76, 1, 1);
    Call6(Func_0200882c, 72, 75, 74, 76, 1, 1);
    Call6(Func_02008858, 7, 75, 1, 1, 6, 75);
    Call6(Func_0200886c, 8, 70, 3, 1, 8, 71);
    Call6(Func_02008880, 8, 70, 2, 1, 9, 72);
    Call6(Func_02008892, 8, 70, 2, 1, 9, 73);
    Call6(Func_020088a4, 11, 66, 1, 1, 8, 73);
    Call6(Func_020088b6, 12, 66, 1, 4, 11, 73);
    Call6(Func_020088ca, 25, 0, 1, 1, 6, 74);
    Func_020088a6();
}
