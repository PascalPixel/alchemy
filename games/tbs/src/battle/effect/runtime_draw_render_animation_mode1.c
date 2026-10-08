//--------------------------------------------------------------------------
// runtime_draw_render_animation_mode1.c
// バトル演出：粒が昇る演出 (embers.c) の二つの入口
//--------------------------------------------------------------------------

#include "types.h"
#include "battle_efx.h"
#include "scene.h"

//--------------------------------------------------------------------------
// mode 1 : 使う者の側から出し、LightningBolt のパレットで描く
//--------------------------------------------------------------------------
void BattleFx_RenderAnimationMode1(s32 effect)
{
	BattleEffect_RunPaletteParticles((struct BattleEffectArgument *)effect, 1);
}

//--------------------------------------------------------------------------
// mode 0 : 8 体の隊列を出して動かし、最後にその位置で爆発を起こす
//--------------------------------------------------------------------------
void BattleFx_RenderAnimationMode0(s32 effect)
{
	BattleEffect_RunPaletteParticles((struct BattleEffectArgument *)effect, 0);
}
