#include "types.h"

void Func_02000f92();
void Func_020010d8(s32, s32, s32);

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Func_020010d8(actor, x, y);
}

void ConfigureAndPlaceActorOneHundredTwo(void)
{
    s32 a = 3, b = 26;
    Func_02000f92(3, 32, 1, 1, a, b);
    PlaceActor(102, 0x00380000, 0x01a80000);
}
