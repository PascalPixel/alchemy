#include "types.h"
#include "scene.h"
#include "abi/battle/effects/seven_mode/run_mode5.h"

void BattleFx_RunSevenModeMode5(s32 arg0)
{
    Battle_Apply(arg0, 5);
}
