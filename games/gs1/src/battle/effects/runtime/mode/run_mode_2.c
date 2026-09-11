#include "types.h"
#include "scene.h"
#include "abi/battle/effects/runtime/mode/run_mode_2.h"

void BattleFx_RunMode2(s32 arg0)
{
    Battle_Apply(arg0, 2);
}
