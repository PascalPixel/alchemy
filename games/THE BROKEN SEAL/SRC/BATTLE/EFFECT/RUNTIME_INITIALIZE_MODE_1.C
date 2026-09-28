#include "TYPES.H"
#include "SCENE.H"
void BattleEffect_RunParticleStreams(s32, s32);

/* battle/effects/runtime/initialize_mode_1.c */
void BattleFx_InitializeMode1(s32 arg0)
{
    BattleEffect_RunParticleStreams(arg0, 1);
}
