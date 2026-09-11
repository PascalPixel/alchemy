#include "types.h"
#include "scene.h"

/* battle/effects/runtime/init/initialize_default_mode.c */
void BattleFx_InitializeDefaultMode(s32 arg0)
{
    Battle_Apply(arg0, 0);
}

/* battle/effects/runtime/init/initialize_mode_1.c */
void BattleFx_InitializeMode1(s32 arg0)
{
    Battle_Apply(arg0, 1);
}
