.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Battle_GetObjectTableValueFar
	.thumb_func
Battle_GetObjectTableValueFar:
	overlay_veneer Func_0811c37c
	.global BattleMotion_ApproachTargetFar
	.thumb_func
BattleMotion_ApproachTargetFar:
	overlay_veneer BattleMotion_ApproachTarget
	.global Func_08118080
	.thumb_func
Func_08118080:
	overlay_veneer Func_0811c01c
