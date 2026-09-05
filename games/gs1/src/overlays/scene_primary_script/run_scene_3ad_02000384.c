#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_02001e7e();
void Func_02001e88();
void Func_02001eaa();
void Func_02001ebe();
void Func_02001ec6();
void Func_02001ed0();
void Func_02001eea();
void Func_02001efc();
void Func_02001f0c();
void Func_02001f26();
void Func_02001f38();

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
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene3ad_02000384(void)
{
    u32 i;
    s32 record;

    Call6(Func_02001e88, 41, 87, 2, 5, 21, 59);
    Func_02001e7e(4);
    Call6(Func_02001eaa, 2, 93, 1, 1, 24, 62);
    Call6(Func_02001ebe, 2, 94, 1, 1, 21, 55);
    Call6(Func_02001ed0, 43, 87, 2, 5, 21, 58);
    Func_02001ec6(4);
    Call6(Func_02001eea, 3, 93, 1, 1, 24, 62);
    Call6(Func_02001efc, 1, 94, 1, 1, 21, 55);
    Call6(Func_02001f0c, 41, 87, 2, 5, 21, 58);
    Call6(Func_02001f26, 21, 11, 2, 2, 21, 13);
    Call6(Func_02001f38, 19, 17, 1, 1, 21, 14);
}
