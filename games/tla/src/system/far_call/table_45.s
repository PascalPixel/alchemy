.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Summon_IsEntrySecondaryFlaggedFar
	.thumb_func
Summon_IsEntrySecondaryFlaggedFar:
	overlay_veneer Summon_IsEntrySecondaryFlagged
	.global Battle_GetSpriteFlagHighBitsFar
	.thumb_func
Battle_GetSpriteFlagHighBitsFar:
	overlay_veneer Battle_GetSpriteFlagHighBits
	.global Summon_IsEntryFlaggedFar
	.thumb_func
Summon_IsEntryFlaggedFar:
	overlay_veneer Summon_IsEntryFlagged
