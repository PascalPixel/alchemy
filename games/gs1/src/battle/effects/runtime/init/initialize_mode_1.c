#include "types.h"
#include "scene.h"
#include "abi/battle/effects/runtime/init/initialize_mode_1.h"

void BattleFx_InitializeMode1(s32 arg0)
{
    Battle_Apply(arg0, 1);
}
