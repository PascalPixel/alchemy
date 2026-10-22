.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global BattleEffect_HasMatchingEvent5Far
	.thumb_func
BattleEffect_HasMatchingEvent5Far:
	overlay_veneer BattleEffect_HasMatchingEvent5
	.global BattleEffect_HasTriggerFar
	.thumb_func
BattleEffect_HasTriggerFar:
	overlay_veneer BattleEffect_HasTrigger
	.global Func_080c8518
	.thumb_func
Func_080c8518:
	overlay_veneer Object_ReplaceResourceEntry
