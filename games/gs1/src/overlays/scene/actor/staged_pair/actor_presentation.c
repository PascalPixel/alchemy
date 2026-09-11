#include "types.h"

#define ActorPresentation_RunActorElevenRecoveryScene Func_02000f44
#define ActorPresentation_SelectActorNineScript Func_02001040
#define ActorPresentation_RunActorEightThresholdScene Func_020010fc
#define ActorPresentation_RunActorNineThresholdScene Func_02001158
void Func_02002464();
void Func_0200247e();
void *Func_020024c4();
void Func_020024c6();
void Func_020024cc();
void Func_020024d0();
void Func_020024e6();
void Func_020024e8();
void Func_020024f0();
void Func_020024f6();
void Func_02002504();
void Func_02002506();
void Func_02002516();
void Func_0200251e();
s32 Func_02002544();
s32 Func_0200254e();
void Func_020025e8();
void Func_020025f0();
void Func_02002608();
void Func_02002314();
void Func_020025fc();
void Func_02002624();
void Func_02002638();
void Func_0200263a();
s32 *Func_02002642();
void Func_0200265e();
void Func_0200264c();
void Func_0200267a();
s32 *Func_02002698();
void Func_0200269a();
void Func_020026b6();
void Func_020026da();
void Func_020026f4();
void Func_02002702(s32, s32, s32);
void Func_02002712();
void Func_02002780();

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Func_02002702(actor, x, y);
}

void ActorPresentation_RunActorElevenRecoveryScene(void)
{
    Func_02002464();
    Func_020024c6(11, 0, 0);
    Func_020024d0(0, 11, 0);
    Func_020024d0_a(0, 1);
    Func_0200247e(10);
    Func_020024f0(0, 11, 0);
    Func_02002506(0x23d9);
    Func_0200251e(11, 0);
    Func_020024f6(11, 2);
    {
        s16 *position = Func_020024c4(0);

        if (position != 0)
            Func_020024e6(11, position[5], position[9]);
    }
    Func_02002504(11);
    Func_02002516(11, 0, 0);
    Func_020024cc(20);
    Func_020024c4_a(2464);
    Func_020024e8();
}

void ActorPresentation_SelectActorNineScript(void)
{
    if (Func_02002544(2384) != 0 && Func_0200254e(2415) == 0)
        Func_020025e8(9173);
    else
        Func_020025f0(9170);
    Func_02002608(9, 0);
}

void ActorPresentation_RunActorEightThresholdScene(void)
{
    Func_02002638(8);
    Func_02002624();
    {
        s32 *actor = Func_02002642(8);

        if ((actor[2] >> 20) <= 30) {
            Func_02002314(8);
            {
                s32 x = 27;
                s32 y = 19;

                Func_020025fc(29, 19, 1, 1, x, y);
            }
            Func_0200263a(2466);
        }
    }
    Func_0200265e();
}

void ActorPresentation_RunActorNineThresholdScene(void)
{
    Func_0200267a();
    if ((Func_02002698(9)[2] >> 20) > 42) {
        s32 x = 107;
        s32 y = 17;

        Func_0200264c(108, 17, 1, 1, x, y);
        Func_0200269a(8);
        Func_020026f4(9, 0, 0);
        PlaceActor(10, 45613056, 18874368);
        Func_02002712(10, 3);
        Func_02002780(154);
        Func_020026b6(2469);
    }
    Func_020026da();
}
