#include "types.h"

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
