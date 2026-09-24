#include "TYPES.H"

struct Probe {
    s32 word[6];
};

void Object_SetModeById(s32 id, s32 mode);
void GameFlag_SetBitFar(s32 flag);
#define HaidiaDou_RunLoweredActorScene Func_020015cc

s32 Func_02001b72();
s32 Func_02001d32(struct Probe *probe);
void Func_02001ede(struct Probe probe);
void Func_0200327e();
void Func_020032be();
void Func_020032cc();
void Func_020032e2();
s32 Func_020032ea();
void Func_020032ec();
void Func_020032f4();
void Func_020032f6();
void Func_020032f8();
void Func_02003324();
void Func_02003340();
void Func_0200337c();

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

/* Haidia Cave: move the probed actor, lower actor 11 and fill the cleared
 * rectangle; at column 20 set flag 0x205, otherwise set 0x204 and copy the
 * opened cells. */
void HaidiaDou_RunLoweredActorScene(void)
{
    s32 record;
    struct Probe probe;

    Func_0200327e();
    if (Func_02001d32(&probe) != 0) {
        Func_02001ede(probe);
        Object_SetModeById(11, 3);
        Call3(Func_020032cc, 11, 0x4000, 0x8000);
        Call3(Func_020032f8, 11, 0, -16);
        Func_020032be(45);
        Func_0200337c(240);
        Func_02003324(11, 8);
        {
            u8 *obj = (u8 *)Func_020032ea(11);
            s32 two = 2;
            s32 zero = 0;

            obj[35] = two;
            Func_02001b72(0, 13, (probe.word[4] >> 20) - 1, 4, two, zero);
        }
        if ((probe.word[4] >> 20) == 20) {
            Call1(Func_020032ec, 0x205);
        } else {
            Call1(GameFlag_SetBitFar, 0x204);
            {
                s32 column = 14;

                Func_020032e2(14, 17, 2, 1, column, 16);
                Func_020032f4(14, 13, 1, 1, column, 15);
            }
        }
    }
    Func_02003340();
}
