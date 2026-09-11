#include "types.h"
#include "scene.h"
#include "abi/battle/effects/five_mode/run_mode1_b.h"

void BattleFx_RunFiveModeMode1B(s32 arg0)
{
    Battle_Apply(arg0, 1);
}
