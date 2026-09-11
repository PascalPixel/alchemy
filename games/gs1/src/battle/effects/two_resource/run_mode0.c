#include "types.h"
#include "scene.h"
#include "abi/battle/effects/two_resource/run_mode0.h"

void BattleFx_RunTwoResourceMode0(s32 arg0)
{
    Battle_Apply(arg0, 0);
}
