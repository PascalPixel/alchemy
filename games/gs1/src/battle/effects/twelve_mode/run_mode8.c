#include "types.h"
#include "scene.h"
#include "abi/battle/effects/twelve_mode/run_mode8.h"

void BattleFx_RunTwelveModeMode8(s32 arg0)
{
    Battle_Apply(arg0, 8);
}
