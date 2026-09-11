#include "types.h"
#include "scene.h"
#include "abi/battle/effects/two_mode_b/run_mode0.h"

void BattleFx_RunTwoModeBMode0(s32 arg0)
{
    Battle_Apply(arg0, 0);
}
