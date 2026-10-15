#include "trial_road.h"

extern const u8 gData_02007734[];

void Func_02000cc0(void)
{
    Func_02004418(gData_02007734);
}

void Func_02000cd0(s32 phase)
{
    if (phase == 3)
        Func_020045ac(gData_02007734, 15);
}

void Func_02000ce4(s32 phase)
{
    if (phase == 3)
        Func_020045ac(gData_02007734, 16);
}
