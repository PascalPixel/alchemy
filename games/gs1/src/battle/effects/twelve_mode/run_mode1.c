#include "types.h"
#include "scene.h"
#include "abi/battle/effects/twelve_mode/run_mode1.h"

void BattleFx_RunTwelveModeMode1(s32 arg0)
{
    Battle_Apply(arg0, 1);
}
