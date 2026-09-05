#include "types.h"

s32 Func_02001d2e(s32);
void Func_02001d40(s32);
void Func_02001d9e(s32, const void *);
void Func_02001dc6(s32, s32, s32);

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Func_02001dc6(actor, x, y);
}

void PlaceActorTwelveWhenFlagClear(void)
{
    if (Func_02001d2e(2487) == 0) {
        Func_02001d40(526);
        PlaceActor(12, 240 << 15, 206 << 18);
        Func_02001d9e(12, (const void *)0x020097a8);
    }
}
