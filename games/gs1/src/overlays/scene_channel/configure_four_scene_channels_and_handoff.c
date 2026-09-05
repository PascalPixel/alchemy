#include "types.h"

void Func_02002f4e(s32, s32, s32);
void Func_02002f5a(s32, s32, s32);
void Func_02002f66(s32, s32, s32);
void Func_02002f72(s32, s32, s32);
void Func_02002ec4(s32);

static __inline__ void ConfigureSecond(s32 channel, s32 value, s32 zero)
{
    Func_02002f5a(channel, value, zero);
}

static __inline__ void ConfigureThird(s32 channel, s32 value, s32 zero)
{
    Func_02002f66(channel, value, zero);
}

static __inline__ void ConfigureFourth(s32 channel, s32 value, s32 zero)
{
    Func_02002f72(channel, value, zero);
}

void ConfigureFourSceneChannelsAndHandoff(s32 handoff)
{
    Func_02002f4e(0, 0x6000, 0);
    ConfigureSecond(1, 0xe000, 0);
    ConfigureThird(2, 0x2000, 0);
    ConfigureFourth(3, 0xa000, 0);
    if (handoff != 0) {
        Func_02002ec4(handoff);
    }
}
