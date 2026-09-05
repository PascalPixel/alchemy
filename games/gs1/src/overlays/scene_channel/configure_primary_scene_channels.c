#include "types.h"

void Func_02003ec4(s32, s32, s32);
void Func_02003ed0(s32, s32, s32);
void Func_02003edc(s32, s32, s32);

static __inline__ void ConfigureFirst(s32 channel, s32 value, s32 zero)
{
    Func_02003ec4(channel, value, zero);
}

static __inline__ void ConfigureSecond(s32 channel, s32 value, s32 zero)
{
    Func_02003ed0(channel, value, zero);
}

static __inline__ void ConfigureThird(s32 channel, s32 value, s32 zero)
{
    Func_02003edc(channel, value, zero);
}

void ConfigurePrimarySceneChannels(void)
{
    ConfigureFirst(1, 0xe000, 0);
    ConfigureSecond(2, 0xa000, 0);
    ConfigureThird(3, 0x8000, 0);
}
