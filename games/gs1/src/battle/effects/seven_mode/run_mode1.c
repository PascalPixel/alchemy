#include "types.h"
#include "scene.h"
#include "abi/battle/effects/seven_mode/run_mode1.h"

void BattleFx_RunSevenModeMode1(s32 arg0)
{
    Battle_Apply(arg0, 1);
}
