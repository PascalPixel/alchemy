#include "TYPES.H"

void Func_02003f4c();
void Func_02003f76();
void Func_02004012();
void Func_0200403a();
void Func_02004062();
void Func_02004068();
void Func_020040ac();
s32 Func_020040bc();
void Func_020040c0();
void Func_02004118();
void Func_02004192();
void Func_02004238();
void Func_020042a0();

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void KorosseoKabe_SpinActorAway(void)
{
    s32 rec2;
    s32 frames;
    s32 spin;
    s32 frames2;
    s32 spin2;
    u8 *p6;

    rec2 = Value1(Func_020040bc, 30);
    p6 = *(s32 *)(rec2 + 80);
    Call1(Func_02004068, 0x330);
    *(s32 *)(rec2 + 52) = 0x1999;
    *(s32 *)(rec2 + 48) = 0x13333;
    Func_02004238(227);
    Call4(Func_02004012, rec2, 0x1500000, 0xa0000, 0x1080000);
    for (spin = 0, frames = 9; frames >= 0; frames--) {
        *(u16 *)((s32)p6 + 30) -= spin;
        Func_02003f4c(1);
        spin += 36;
    }
    Call4(Func_0200403a, rec2, 0x14a0000, -0x100000, 0x1080000);
    for (spin2 = 0x168, frames2 = 21; frames2 >= 0; frames2--) {
        *(u16 *)((s32)p6 + 30) -= spin2;
        Func_02003f76(1);
        spin2 += 36;
    }
    Func_02004062(rec2);
    Func_02004118(2);
    Func_020042a0(240);
    {
        s32 shown = 0;
    
        *(u16 *)((s32)p6 + 30) = shown;
    }
    Func_02004192(30, 4);
    *(s32 *)(rec2 + 8) = 0x1500000;
    *(s32 *)(rec2 + 12) = -0x80000;
    *(s32 *)(rec2 + 16) = 0x1080000;
    *(s32 *)(rec2 + 40) = 0;
    *(s32 *)(rec2 + 36) = 0;
    Call6(Func_020040ac, 19, 16, 1, 1, 20, 16);
    Call6(Func_020040c0, 20, 80, 1, 1, 21, 80);
}
