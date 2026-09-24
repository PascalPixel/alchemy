#include "TYPES.H"

#define SoruSekizo_RunEventSequence Func_02001b44

void Func_02003fb0();
void Func_02003fe0();
s32 Func_02003fe8();
void Func_02003fea();
s32 Func_0200401c();
void Func_0200402a();
void Func_02004038();
void Func_02004042();
void Func_0200405e();
void Func_02004076();
void Func_020040c6();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

void SoruSekizo_RunEventSequence(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4)
{
    s32 p10;
    s32 p10b;
    s32 p10c;
    s32 p8;
    s32 p8b;
    s32 p9;
    s32 p9b;
    s32 record;

    p9 = a0;
    p8 = a1;
    p10 = a2;
    Func_02003fb0();
    Func_020040c6(185);
    Call3(Func_02003fe0, p9, 0x3333, 0x1999);
    Call3(Func_02003fea, 0, 0x3333, 0x1999);
    *(u8 *)(Func_02003fe8(p9) + 90) &= 254;
    Func_02004042(0, 8);
    Value3(Func_0200401c, 0, ((a3 << 4) + 8), ((a4 << 4) + 8));
    p8b = ((s32)p8 << 4);
    p10b = ((s32)p10 << 4);
    Func_02004038(p9, (p8b + 8), (p10b + 8));
    Func_0200405e(p9);
    Func_02004076(0, 1);
    Func_0200402a();
    p9b = ((a3 << 4) + 8);
    p10c = ((a4 << 4) + 8);
}
