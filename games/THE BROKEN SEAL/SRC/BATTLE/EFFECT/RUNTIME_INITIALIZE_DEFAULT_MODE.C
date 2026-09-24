#include "TYPES.H"
#include "SCENE.H"

/* battle/effects/runtime/initialize_default_mode.c */
void BattleFx_InitializeDefaultMode(s32 arg0)
{
    BattleEffect_RunParticleStreams(arg0, 0);
}
