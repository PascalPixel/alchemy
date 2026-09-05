#include "types.h"

#define FieldScene_RunScene387SequenceD Func_0200043c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
void Func_020007f8();
void Func_0200143c();
void Func_02001454();
void Func_02001470();
void Func_02001476();
void Func_0200147c();
s32 Func_02001494();
void Func_020014aa();
void Func_020014c6();
void Func_020014d6();
void Func_020014e0();
void Func_02001512();
s32 Func_0200151c();
void Func_02001536();
s32 Func_02001538();
s32 Func_02001544();
s32 Func_02001550();
void Func_02001570();
void Func_02001576();
void Func_020015a8();
void Func_020015b2();
void Func_020015c8();
void Func_020015d0();
void Func_020015ec();
void Func_020015f4();
void Func_020015fe();
void Func_0200160c();
void Func_02001612();
void Func_02001630();
void Func_02001698();
void Func_020016d8();

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}



void FieldScene_RunScene387SequenceD(void)
{
    u8 *p5;
    s32 v5;

    p5 = *(u8 **)Data_03001ebc;
    Func_02001570();
    Func_020015d0(0, 8);
    Func_02001576(20);
    Call3(Func_020015a8, 0, 0x3333, 0x1999);
    Call3(Func_020015b2, 9, 0x3333, 0x1999);
    Func_02001698(185);
    v5 = (11 - (*(s16 *)(p5 + 0x16c) << 1)) << 4;
    Func_020015f4(0, v5, 0);
    Func_020015fe(9, v5, 0);
    Func_0200160c(0);
    Func_02001612(9);
    Func_020015c8(20);
    Func_02001630(0, 1);
    Func_020007f8();
    Func_020016d8();
    Func_020015ec();
}
