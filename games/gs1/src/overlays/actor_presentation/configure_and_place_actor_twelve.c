#include "types.h"
/* resource_3b4 owner at 0x020011d8, 44 bytes. */
void Func_020036be();
void Func_02003774();

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Func_02003774(actor, x, y);
}

#define ConfigureAndPlaceActorTwelve Func_020011d8

void ConfigureAndPlaceActorTwelve(void)
{
    s32 a = 25, b = 48;
    Func_020036be(24, 48, 1, 2, a, b);
    PlaceActor(12, 0x00080000, 0x00080000);
}
