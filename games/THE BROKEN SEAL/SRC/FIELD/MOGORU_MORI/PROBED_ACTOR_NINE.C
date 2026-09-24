#include "TYPES.H"

#define MogoruMori_RunProbedActorNineScene Func_02000f94

struct Resource39fProbe {
    s32 word[6];
};

void Func_02003d3e(void);
s32 Func_02001416(struct Resource39fProbe *probe);
void Func_020015c2(struct Resource39fProbe probe);
void Func_02003d64(s32 flag);
void Func_02003dd4(s32 actor, s32 animation);
void Func_02003daa(s32 actor, s32 speed, s32 acceleration);
void Func_02003dd6(s32 actor, s32 dx, s32 dz);
void Func_02003d94(s32 frames);
void Func_02003dfc(s32 actor, s32 animation);
void Func_02003eba(s32 cue);
void Func_02003e4a(s32 actor, s32 value);
u8 *Func_02003dd0(s32 actor);
void Func_02003d7a(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_02003dde(void);

/* Spelled through this wrapper, the call loads r0 last of its arguments. */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Mogoru Forest: when the probe finds actor 9 in map column 26, set flag
 * 0x310, step actor 9 back, play cue 240 and copy the cleared cells. */
void MogoruMori_RunProbedActorNineScene(void)
{
    struct Resource39fProbe probe;
    s32 fifth;
    s32 sixth;

    Func_02003d3e();
    if (Func_02001416(&probe) != 0) {
        Func_020015c2(probe);
        if (probe.word[1] == 9 && (probe.word[4] >> 20) == 26) {
            Func_02003d64(0x310);
            Func_02003dd4(9, 3);
            Call3(Func_02003daa, 9, 0x4000, 0x8000);
            Func_02003dd6(9, 0, -16);
            Func_02003d94(45);
            Func_02003dfc(9, 8);
            Func_02003eba(240);
            Func_02003e4a(9, 1);
            Func_02003dd0(9)[35] = 2;
            fifth = 31;
            sixth = 25;
            Func_02003d7a(38, 27, 4, 2, fifth, sixth);
        }
    }
    Func_02003dde();
}
