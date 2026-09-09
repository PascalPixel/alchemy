#include "types.h"

#define ConfigurePrimarySceneChannels Func_02001958
#define ConfigureSecondarySceneChannels Func_02001984
void Func_02003ec4(s32, s32, s32);
void Func_02003ed0(s32, s32, s32);
void Func_02003edc(s32, s32, s32);
void Func_02003ef0(s32, s32, s32);
void Func_02003efc(s32, s32, s32);
void Func_02003f08(s32, s32, s32);

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

static __inline__ void ConfigureFirst_02003ef0(s32 channel, s32 value, s32 zero)
{
    Func_02003ef0(channel, value, zero);
}

static __inline__ void ConfigureSecond_02003efc(s32 channel, s32 value, s32 zero)
{
    Func_02003efc(channel, value, zero);
}

static __inline__ void ConfigureThird_02003f08(s32 channel, s32 value, s32 zero)
{
    Func_02003f08(channel, value, zero);
}

void ConfigurePrimarySceneChannels(void)
{
    ConfigureFirst(1, 0xe000, 0);
    ConfigureSecond(2, 0xa000, 0);
    ConfigureThird(3, 0x8000, 0);
}

void ConfigureSecondarySceneChannels(void)
{
    ConfigureFirst_02003ef0(1, 0xc000, 0);
    ConfigureSecond_02003efc(2, 0xc000, 0);
    ConfigureThird_02003f08(3, 0xa000, 0);
}
