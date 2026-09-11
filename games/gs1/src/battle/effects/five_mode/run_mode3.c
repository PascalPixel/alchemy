#include "types.h"
#include "scene.h"
#include "abi/battle/effects/five_mode/run_mode3.h"

void BattleFx_RunFiveModeMode3(s32 arg0)
{
    Battle_Apply(arg0, 3);
}
