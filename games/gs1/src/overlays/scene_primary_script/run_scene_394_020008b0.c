#include "types.h"

#define FieldScene_RunScene394_020008b0 Func_020008b0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u16 *Data_020092c4;
void Func_02001952();
void Func_02001958();
void Func_02001982();
void Func_0200198a();
void Func_0200199a();
void Func_020019a4();
void Func_020019b4();
void Func_020019ba();
void Func_020019cc();
void Func_020019ce();
void Func_020019d2();
void Func_020019de();
void Func_020019ea();
void Func_020019f4();
void Func_02001a02();
void Func_02001a08();
void Func_02001a42();
void Func_02001a48();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void FieldScene_RunScene394_020008b0(void)
{
    Func_02001952();
    Func_0200199a(0, 8);
    Func_02001958(6);
    Func_020019de(239);
    Call3(Func_0200198a, 8, 0x8000, 0x3333);
    Func_020019ba(8, 2);
    Func_020019a4(8, 104, 176);
    Func_02001982(6);
    Func_020019d2(0, 2);
    Call3(Func_020019b4, 0, 0x4ccc, 0x3333);
    Func_020019ce(0, 8, 0);
    Func_020019a4(24);
    Func_020019f4(0, 1);
    Func_020019ea(8);
    Func_02001a02(8, 1);
    Call1(Func_02001a42, 0x120);
    Func_02001a48(213);
    Call6(Func_020019ba, 5, 9, 1, 4, 4, 9);
    Call6(Func_020019cc, 0, 0, 1, 4, 6, 9);
    *Data_020092c4 = 0;
    Func_02001a08();
}
