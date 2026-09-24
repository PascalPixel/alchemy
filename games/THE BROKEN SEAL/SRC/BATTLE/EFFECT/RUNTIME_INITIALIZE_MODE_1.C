#include "TYPES.H"
#include "SCENE.H"

/* battle/effects/runtime/initialize_mode_1.c */
void BattleFx_InitializeMode1(s32 arg0)
{
    BattleEffect_RunParticleStreams(arg0, 1);
}
