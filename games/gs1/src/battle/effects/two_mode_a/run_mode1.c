#include "types.h"
#include "scene.h"
#include "abi/battle/effects/two_mode_a/run_mode1.h"

void BattleFx_RunTwoModeAMode1(s32 arg0)
{
    Battle_Apply(arg0, 1);
}
