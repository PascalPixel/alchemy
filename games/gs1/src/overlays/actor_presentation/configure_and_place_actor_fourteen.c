#include "types.h"

void Func_02001d3e();
void Func_02001e88();
void Func_02001dee(s32, s32, s32);

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Func_02001dee(actor, x, y);
}

#define ConfigureAndPlaceActorFourteen Func_02000870

void ConfigureAndPlaceActorFourteen(void)
{
    s32 a = 21, b = 9;
    Func_02001d3e(85, 9, 1, 1, a, b);
    Func_02001e88(100, 0, 0);
    PlaceActor(14, 0x01580000, 0x00980000);
}
