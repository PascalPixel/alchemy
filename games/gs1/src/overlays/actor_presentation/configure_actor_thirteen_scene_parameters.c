#include "types.h"

void Func_02001ec8(s32, s32, s32);
void Func_02001e92(s32, s32, s32);
void Func_02001ee2(s32, s32);

static __inline__ void ConfigureFirst(s32 actor, s32 angle, s32 zero)
{
    Func_02001ec8(actor, angle, zero);
}

static __inline__ void ConfigureSecond(s32 actor, s32 mode, s32 zero)
{
    Func_02001e92(actor, mode, zero);
}

static __inline__ void ConfigureThird(s32 actor, s32 value)
{
    Func_02001ee2(actor, value);
}

#define ConfigureActorThirteenSceneParameters Func_020009dc

void ConfigureActorThirteenSceneParameters(void)
{
    ConfigureFirst(13, 256, 0);
    ConfigureSecond(13, 2, 0);
    ConfigureThird(12, 40);
}
