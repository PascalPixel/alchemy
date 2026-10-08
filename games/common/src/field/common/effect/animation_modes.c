//--------------------------------------------------------------------------
// animation_modes.c
// 演出：バースト演出の入口
//--------------------------------------------------------------------------

#include "effect_step.h"

void BattlePres_RunBurstScene(struct EffectStep *, s32);

//--------------------------------------------------------------------------
// 演出ごとの入口。バーストの型を BattlePres_RunBurstScene に渡す
// step : 演出の引数
//--------------------------------------------------------------------------
void EffectStep_RunAnimationMode6Or7Or8(struct EffectStep *step)
{
	if (step->variant == 0)
	{
		BattlePres_RunBurstScene(step, 6);
		return;
	}
	if (step->variant == 1)
	{
		BattlePres_RunBurstScene(step, 7);
		return;
	}
	BattlePres_RunBurstScene(step, 8);
}

void EffectStep_RunAnimationMode0(struct EffectStep *step)
{
	BattlePres_RunBurstScene(step, 0);
}

void EffectStep_RunAnimationMode0Alternate(struct EffectStep *step)
{
	BattlePres_RunBurstScene(step, 0);
}

void EffectStep_RunAnimationMode1(struct EffectStep *step)
{
	BattlePres_RunBurstScene(step, 1);
}

void EffectStep_RunAnimationMode2(struct EffectStep *step)
{
	BattlePres_RunBurstScene(step, 2);
}

void EffectStep_RunAnimationMode3Or4Or5(struct EffectStep *step)
{
	if (step->variant == 0)
	{
		BattlePres_RunBurstScene(step, 3);
		return;
	}
	if (step->variant == 1)
	{
		BattlePres_RunBurstScene(step, 4);
		return;
	}
	BattlePres_RunBurstScene(step, 5);
}
