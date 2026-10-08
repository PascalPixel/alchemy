//--------------------------------------------------------------------------
// RENDER_MODE.C
// バトル演出：粒子の演出の入口
//--------------------------------------------------------------------------

#include "TYPES.H"
#include "BATTLE.H"
#include "BATTLE_EFFECT_WORK.H"

//--------------------------------------------------------------------------
// 演出ごとの入口。粒子の型（色と数）を BattleFx_RenderMode に渡す
// effect : 演出の引数
//--------------------------------------------------------------------------
void BattleFx_RenderMode0Or1(struct BattleEffectArgument *effect)
{
	if (effect->variant == 0)
	{
		BattleFx_RenderMode(effect, 0);
		return;
	}
	BattleFx_RenderMode(effect, 1);
}

void BattleFx_RenderMode1(struct BattleEffectArgument *effect)
{
	BattleFx_RenderMode(effect, 1);
}

void BattleFx_RenderMode2(struct BattleEffectArgument *effect)
{
	BattleFx_RenderMode(effect, 2);
}

void BattleFx_RenderMode3(struct BattleEffectArgument *effect)
{
	BattleFx_RenderMode(effect, 3);
}

void BattleFx_RenderMode4(struct BattleEffectArgument *effect)
{
	BattleFx_RenderMode(effect, 4);
}

void BattleFx_RenderMode5(struct BattleEffectArgument *effect)
{
	BattleFx_RenderMode(effect, 5);
}

void BattleFx_RenderMode7(struct BattleEffectArgument *effect)
{
	BattleFx_RenderMode(effect, 7);
}

void BattleFx_RenderMode6(struct BattleEffectArgument *effect)
{
	BattleFx_RenderMode(effect, 6);
}
