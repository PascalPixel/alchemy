#include "local.h"

void Func_020047f0(void)
{
    ObjectMotion_SetSpeedParameters_(12, 0x19999, 0xcccc);
    ObjectMotion_SetPositionAndReset_(12, 496, 392);
    ObjectMotion_ArmCallback_(12, 0x8000, 0);
    Func_02005298(0x400c, 0);
}
