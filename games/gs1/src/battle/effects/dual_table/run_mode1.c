#include "types.h"
#include "scene.h"
#include "abi/battle/effects/dual_table/run_mode1.h"

void BattleFx_RunDualTableMode1(s32 arg0)
{
    Battle_Apply(arg0, 1);
}
