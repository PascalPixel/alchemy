#include "types.h"

void Func_02003ef0(s32, s32, s32);
void Func_02003efc(s32, s32, s32);
void Func_02003f08(s32, s32, s32);

static __inline__ void ConfigureFirst(s32 channel, s32 value, s32 zero)
{
    Func_02003ef0(channel, value, zero);
}

static __inline__ void ConfigureSecond(s32 channel, s32 value, s32 zero)
{
    Func_02003efc(channel, value, zero);
}

static __inline__ void ConfigureThird(s32 channel, s32 value, s32 zero)
{
    Func_02003f08(channel, value, zero);
}

void ConfigureSecondarySceneChannels(void)
{
    ConfigureFirst(1, 0xc000, 0);
    ConfigureSecond(2, 0xc000, 0);
    ConfigureThird(3, 0xa000, 0);
}
