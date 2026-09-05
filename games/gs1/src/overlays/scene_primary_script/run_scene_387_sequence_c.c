#include "types.h"

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

void FieldScene_RunScene387SequenceC(void)
{
    s32 record;
    s32 v3;
    s32 v5;

    record = Value1(Func_0200151c, 10);
    v3 = *(volatile s32 *)(record + 8) / 0x100000;
    if (v3 == 23) {
        Func_02001512(10);
        *(u8 *)(Func_02001538(10) + 35) = 2;
        v5 = 0;
        *(u8 *)(Func_02001544(10) + 85) = v5;
        Func_020014d6(Func_02001550(10), 0);
        Call6(Func_020014e0, 54, 17, 1, 1, v3, 17);
        Call1(Func_02001536, 0x863);
    }
}
