#include "types.h"
#include "scene.h"
#include "abi/battle/effects/runtime/mode/run_mode_3.h"

void BattleFx_RunMode3(s32 arg0)
{
    Battle_Apply(arg0, 3);
}
