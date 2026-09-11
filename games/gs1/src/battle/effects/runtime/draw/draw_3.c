#include "types.h"
#include "scene.h"

/* battle/effects/runtime/draw/render_animation_mode_1.c */
s32 BattleFx_RenderAnimation(s32, s32);

void BattleFx_RenderAnimationMode1(s32 effect)
{
    BattleFx_RenderAnimation(effect, 1);
}

/* battle/effects/runtime/draw/render_animation_mode_0.c */
s32 BattleFx_RenderAnimation(s32, s32);

void BattleFx_RenderAnimationMode0(s32 effect)
{
    BattleFx_RenderAnimation(effect, 0);
}
