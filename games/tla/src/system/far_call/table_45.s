.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Summon_IsEntrySecondaryFlaggedFar
	.thumb_func
Summon_IsEntrySecondaryFlaggedFar:
	overlay_veneer Summon_IsEntrySecondaryFlagged
	.global BattleClass_GetAttackHopFar
	.thumb_func
BattleClass_GetAttackHopFar:
	overlay_veneer BattleClass_GetAttackHop
	.global BattleClass_IsSmallFar
	.thumb_func
BattleClass_IsSmallFar:
	overlay_veneer BattleClass_IsSmall
