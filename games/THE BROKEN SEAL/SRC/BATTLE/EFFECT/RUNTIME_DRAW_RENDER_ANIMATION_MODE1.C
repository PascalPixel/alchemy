#include "TYPES.H"
#include "BATTLE_EFX.H"
#include "SCENE.H"

void BattleFx_RenderAnimationMode1(s32 effect)
{
    BattleEffect_RunPaletteParticles((struct BattleEffectArgument *)effect, 1);
}

void BattleFx_RenderAnimationMode0(s32 effect)
{
    BattleEffect_RunPaletteParticles((struct BattleEffectArgument *)effect, 0);
}
