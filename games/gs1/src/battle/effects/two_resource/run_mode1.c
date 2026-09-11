#include "types.h"
#include "scene.h"
#include "abi/battle/effects/two_resource/run_mode1.h"

void BattleFx_RunTwoResourceMode1(s32 arg0)
{
    Battle_Apply(arg0, 1);
}
