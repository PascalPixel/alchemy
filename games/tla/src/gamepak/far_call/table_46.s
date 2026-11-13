.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global BattleUnit_GetEffectHeightFar
	.thumb_func
BattleUnit_GetEffectHeightFar:
	overlay_veneer BattleUnit_GetEffectHeight
	.global BattleMotion_ApproachTargetFar
	.thumb_func
BattleMotion_ApproachTargetFar:
	overlay_veneer BattleMotion_ApproachTarget
	.global Func_08118080
	.thumb_func
Func_08118080:
	overlay_veneer BattleMotion_LeapBack
