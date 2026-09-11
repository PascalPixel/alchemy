#include "types.h"
#include "scene.h"
#include "abi/battle/effects/call/call_effect_03_and_stop.h"


void BattleFx_CallEffect03AndStop(void)
{
    Battle_Check();
    EffectRuntime_StopCurrentObject();
}
