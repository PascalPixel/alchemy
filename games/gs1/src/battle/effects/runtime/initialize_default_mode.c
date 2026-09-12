#include "types.h"
#include "scene.h"

/* battle/effects/runtime/initialize_default_mode.c */
void BattleFx_InitializeDefaultMode(s32 arg0)
{
    Battle_Apply(arg0, 0);
}
