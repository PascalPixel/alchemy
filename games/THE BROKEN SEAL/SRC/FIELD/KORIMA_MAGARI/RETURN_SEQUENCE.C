#include "TYPES.H"

#define KorimaMagari_RunReturnSequence Func_020007e0

extern u8 Data_00000001[];
void Func_02001882();
void Func_02001888();
void Func_020018b2();
void Func_020018ba();
void Func_020018ca();
void Func_020018d4();
void Func_020018d6();
void Func_020018e4();
void Func_020018ea();
void Func_020018ee();
void Func_020018fe();
void Func_02001900();
void Func_02001902();
void Func_0200190e();
void Func_0200191c();
void Func_02001926();
void Func_02001934();
void Func_0200193a();
void Func_02001974();
void Func_0200197a();

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

void KorimaMagari_RunReturnSequence(void)
{
    s32 v5;

    Func_02001882();
    Func_020018ca(0, 8);
    Func_02001888(6);
    Func_0200190e(239);
    Call3(Func_020018ba, 8, 0x8000, 0x3333);
    Func_020018ea(8, 2);
    Func_020018d4(8, 72, 176);
    Func_020018b2(6);
    Func_02001902(0, 2);
    Call3(Func_020018e4, 0, 0x4ccc, 0x3333);
    Call3(Func_02001900, 0, -8, 0);
    Func_020018d6(24);
    Func_02001926(0, 1);
    Func_0200191c(8);
    Func_02001934(8, 1);
    Call1(Func_02001974, 0x120);
    v5 = 9;
    Func_0200197a(213);
    Call6(Func_020018ee, 5, 9, 1, 4, 6, v5);
    Call6(Func_020018fe, 0, 0, 1, 4, 4, v5);
    {
        u16 *p = *(u16 **)0x020092c4;
        /* FAKEMATCH: the 1 is a HImode pool constant, as the reference
         * loads it from the literal pool. */
        s32 one = (u16)(u32)Data_00000001;

        *p = one;
    }
    Func_0200193a();
}
