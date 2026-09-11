#include "types.h"
#include "scene.h"
#include "abi/battle/effects/seven_mode/run_mode2.h"

void BattleFx_RunSevenModeMode2(s32 arg0)
{
    Battle_Apply(arg0, 2);
}
