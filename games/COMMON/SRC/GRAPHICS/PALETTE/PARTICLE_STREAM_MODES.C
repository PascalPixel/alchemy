#include "TYPES.H"
#include "BATTLE_EFX.H"

void BattleFx_InitializeDefaultMode(struct BattleEffectArgument *effect)
{
    BattleEffect_RunParticleStreams(effect, 0);
}

void BattleFx_InitializeMode1(struct BattleEffectArgument *effect)
{
    BattleEffect_RunParticleStreams(effect, 1);
}
