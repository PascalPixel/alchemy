#include "TYPES.H"

struct Probe {
    s32 word[6];
};

#define HaidiaDou_RunProbedColumnScene Func_02001390

s32 Func_0200197a();
s32 Func_02001990();
s32 Func_020019a2();
s32 Func_020019b2();
s32 Func_02001afa(struct Probe *probe);
void Func_02001ca6(struct Probe probe);
void Func_020020f2();
void Func_02003046();
s32 Func_0200308c();
s32 Func_02003098();
s32 Func_020030c2();
void Func_020030c6();
void Func_020030ce();
void Func_020030da();
void Func_020030dc();
s32 Func_020030ec();
void Func_020030f4();
void Func_020030fc();
void Func_0200311c();
void Func_02003124();
void Func_02003126();
void Func_0200314e();
void Func_020031f2();

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

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    return f(a0, a1, a2, a3, a4, a5);
}

/* Haidia Cave: when the probe lands in map column 17, lower the probed actor
 * into place, fill the cleared grid rectangles, set flag 0x203 and play cue
 * 240. */
void HaidiaDou_RunProbedColumnScene(void)
{
    s32 record;
    s32 v6;
    s32 v5;
    s32 two;
    struct Probe probe;

    Func_02003046();
    if (Func_02001afa(&probe) != 0) {
        Func_02001ca6(probe);
        if ((probe.word[2] >> 20) == 17) {
            Func_020030c6(probe.word[1], 3);
            v6 = 0;
            *(u8 *)(Func_0200308c(probe.word[1]) + 85) = v6;
            record = Func_02003098(probe.word[1]);
            *(s32 *)(record + 68) = v6;
            Call3(Func_020030ce, probe.word[1], -12, 0);
            Func_020030dc(probe.word[1]);
            Func_020030f4(probe.word[1], 3);
            Func_0200311c(10, 3);
            *(u8 *)(Func_020030c2(probe.word[1]) + 85) = 3;
            Call3(Func_020030fc, probe.word[1], -6, 0);
            Func_020030da(probe.word[1]);
            Func_020020f2();
            Func_02003126(probe.word[1], 8);
            {
                u8 *obj = (u8 *)Func_020030ec(probe.word[1]);

                two = 2;
                obj[35] = two;
            }
            v5 = 4;
            Func_0200197a(0, (probe.word[2] >> 20), ((probe.word[4] >> 20) - 2), 1, v5, v6);
            Func_02001990(2, (probe.word[2] >> 20), ((probe.word[4] >> 20) - 2), 1, v5, v6);
            Func_020019a2(2, 16, 18, 1, two, v6);
            Func_020019b2(0, 16, 16, 1, v5, v6);
            Call1(Func_02003124, 0x203);
            Func_020031f2(240);
        }
    }
    Func_0200314e();
}
