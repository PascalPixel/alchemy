.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global BattleFx_HasMatchingEvent5Far
	.thumb_func
BattleFx_HasMatchingEvent5Far:
	overlay_veneer BattleFx_HasMatchingEvent5
	.global BattleFx_HasTriggerFar
	.thumb_func
BattleFx_HasTriggerFar:
	overlay_veneer BattleFx_HasTrigger
	.global Func_080c8518
	.thumb_func
Func_080c8518:
	overlay_veneer Object_ReplaceResourceEntry
