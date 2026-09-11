#include "types.h"
#include "scene.h"
#include "abi/battle/effects/runtime/init/initialize_default_mode.h"

void BattleFx_InitializeDefaultMode(s32 arg0)
{
    Battle_Apply(arg0, 0);
}
