#include "TYPES.H"
#include "SCENE.H"
void BattleEffect_RunParticleStreams(s32, s32);

/* battle/effects/runtime/initialize_default_mode.c */
void BattleFx_InitializeDefaultMode(s32 arg0)
{
    BattleEffect_RunParticleStreams(arg0, 0);
}
