#include "types.h"
#include "scene.h"
#include "abi/battle/effects/runtime/mode/run_mode_11.h"

void BattleFx_RunMode11(s32 arg0)
{
    Battle_Apply(arg0, 0xB);
}
