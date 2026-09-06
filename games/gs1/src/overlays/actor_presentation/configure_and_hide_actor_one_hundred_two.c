#include "types.h"

void Func_02000fbe();
void Func_02001104(s32, s32, s32);

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Func_02001104(actor, x, y);
}

void Func_0200028c(void)
{
    s32 a = 3, b = 26;
    Func_02000fbe(2, 25, 1, 1, a, b);
    PlaceActor(102, -1, -1);
}
