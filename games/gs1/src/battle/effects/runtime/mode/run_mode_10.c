#include "types.h"
#include "scene.h"
#include "abi/battle/effects/runtime/mode/run_mode_10.h"

void BattleFx_RunMode10(s32 arg0)
{
    Battle_Apply(arg0, 0xA);
}
