#include "TYPES.H"

#define HaidiaBabi_RunActorEightMessageScene Func_0200133c

s32 Func_02002c32();
void Func_02002c4c();
void Func_02002c60();
void Func_02002c6a();
void Func_02002c7e();
void Func_02002cae();
void Func_02002d10();
void Func_02002d12();
void Func_02002d26();
void Func_02002d3e();
void Func_02002d46();
void Func_02002d6a();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

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

/* When flag 0x203 is set, actor 8 walks off and message 0x1c77 plays;
 * otherwise it turns, shows message 0x1c79 and then 0x1c7a. */
void HaidiaBabi_RunActorEightMessageScene(void)
{
    s32 record;
    s32 base5_1c79;

    Func_02002c4c();
    if (Value1(Func_02002c32, 0x203) != 0) {
        Call3(Func_02002d12, 8, 0x10000, 0x2009e6c);
        Func_02002c60(20);
        Call1(Func_02002d26, 0x1c77);
        Func_02002d3e(8, 0);
    } else {
        Func_02002d10(8, 2);
        Func_02002c7e(40);
        do {
            base5_1c79 = 0x1c79;
        } while (0); /* FAKEMATCH: the wrap keeps the message id load after the wait. */
        Func_02002d46(base5_1c79);
        Func_02002d6a(8, 0, 40);
        Func_02002c6a((base5_1c79 + 1), 1);
    }
    Func_02002cae();
}
