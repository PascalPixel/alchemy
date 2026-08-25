#include "types.h"

void Func_02006326(s32, s32, s32);
void Func_020017d6();

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Func_02006326(actor, x, y);
}

#define PlaceActorTwelveAndFinishScene Func_02000c78

void PlaceActorTwelveAndFinishScene(void)
{
    PlaceActor(12, 0x00f80000, 0x01780000);
    Func_020017d6();
}
