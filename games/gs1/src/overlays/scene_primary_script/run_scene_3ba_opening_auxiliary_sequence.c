#include "types.h"

#define FieldScene_RunOpeningAuxiliarySequence Func_0200004c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_0200c41c[];
void Func_02003c7e();
void Func_02003c90();
void Func_02003c9a();
void Func_02003cc0();
void Func_02003cd2();
void Func_02003cf0();
void Func_02003d02();
void Func_02003d14();
void Func_02003daa();
void Func_02003dda();
void Func_02003e0a();

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

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    s32 record;
    s32 base7_200c41c;

    base7_200c41c = (s32)Data_0200c41c;
    switch ((u32)*(volatile s32 *)base7_200c41c) {
    case 66:
        Call6(Func_02003cc0, 92, 31, 2, 2, 50, 38);
        Call6(Func_02003cd2, 92, 31, 2, 2, 54, 38);
        Func_02003dda(16, 10);
        break;
    case 60:
        Call6(Func_02003c7e, 92, 33, 2, 2, 50, 38);
        Call6(Func_02003c90, 92, 33, 2, 2, 54, 38);
        Call6(Func_02003c9a, 50, 25, 6, 1, 50, 12);
        Func_02003daa(16, 11);
        break;
    case 6:
        Call6(Func_02003cc0, 92, 31, 2, 2, 50, 38);
        Call6(Func_02003cd2, 92, 31, 2, 2, 54, 38);
        Func_02003dda(16, 10);
        break;
    case 0:
        Call6(Func_02003cf0, 92, 29, 2, 2, 50, 38);
        Call6(Func_02003d02, 92, 29, 2, 2, 54, 38);
        Func_02003e0a(16, 12);
        Call6(Func_02003d14, 50, 24, 6, 1, 50, 12);
        *(volatile s32 *)base7_200c41c = 120;
        break;
    }
    *(volatile s32 *)base7_200c41c = (*(volatile s32 *)base7_200c41c - 1);
}
