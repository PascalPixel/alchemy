#include "types.h"

void Func_02002f92(s32, s32, s32);
void Func_02002f9e(s32, s32, s32);
void Func_02002faa(s32, s32, s32);
void Func_02002fb6(s32, s32, s32);
void Func_02002f08(s32);

static __inline__ void ConfigureFirst(s32 channel, s32 value, s32 zero)
{
    Func_02002f92(channel, value, zero);
}

static __inline__ void ConfigureSecond(s32 channel, s32 value, s32 zero)
{
    Func_02002f9e(channel, value, zero);
}

static __inline__ void ConfigureThird(s32 channel, s32 value, s32 zero)
{
    Func_02002faa(channel, value, zero);
}

static __inline__ void ConfigureFourth(s32 channel, s32 value, s32 zero)
{
    Func_02002fb6(channel, value, zero);
}

void Func_02001450(s32 handoff)
{
    ConfigureFirst(0, 0xc000, 0);
    ConfigureSecond(1, 0xc000, 0);
    ConfigureThird(2, 0xc000, 0);
    ConfigureFourth(3, 0xc000, 0);
    if (handoff != 0) {
        Func_02002f08(handoff);
    }
}
