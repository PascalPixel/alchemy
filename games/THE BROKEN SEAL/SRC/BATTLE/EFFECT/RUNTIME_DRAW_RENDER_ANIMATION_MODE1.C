#include "TYPES.H"
#include "SCENE.H"

/* battle/effects/runtime/draw/render_animation_mode_1.c */
s32 BattleEffect_RunPaletteParticles(s32, s32);

void BattleFx_RenderAnimationMode1(s32 effect)
{
    BattleEffect_RunPaletteParticles(effect, 1);
}

/* battle/effects/runtime/draw/render_animation_mode_0.c */

void BattleFx_RenderAnimationMode0(s32 effect)
{
    BattleEffect_RunPaletteParticles(effect, 0);
}
