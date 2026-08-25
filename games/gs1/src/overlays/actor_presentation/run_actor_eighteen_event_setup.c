#include "types.h"

void Func_02001632();
void Func_02001672();
void Func_02001686(s32, s32, s32);
void Func_020016e0();
void Func_020016f8();
void Func_02001724();
void Func_02001762(s32, s32, s32);

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Func_02001686(actor, x, y);
}

static __inline__ void SetOffset(s32 actor, s32 offset, s32 zero)
{
    Func_02001762(actor, offset, zero);
}

#define RunActorEighteenEventSetup Func_0200010c

void RunActorEighteenEventSetup(void)
{
    Func_02001632(2491);
    Func_020016e0(0x28b8);
    Func_020016f8(18, 0);
    PlaceActor(18, 65536, 32768);
    SetOffset(18, -16, 0);
    Func_02001724(18, 0, 0);
    Func_02001672(10);
}
