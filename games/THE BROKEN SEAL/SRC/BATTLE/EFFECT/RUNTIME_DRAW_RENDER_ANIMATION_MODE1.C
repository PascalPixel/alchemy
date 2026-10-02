#include "TYPES.H"
#include "BATTLE_EFX.H"
#include "SCENE.H"

/* battle/effects/runtime/draw/render_animation_mode_1.c */

void BattleFx_RenderAnimationMode1(s32 effect)
{
    BattleEffect_RunPaletteParticles((struct BattleEffectArgument *)effect, 1);
}

/* battle/effects/runtime/draw/render_animation_mode_0.c */

void BattleFx_RenderAnimationMode0(s32 effect)
{
    BattleEffect_RunPaletteParticles((struct BattleEffectArgument *)effect, 0);
}
