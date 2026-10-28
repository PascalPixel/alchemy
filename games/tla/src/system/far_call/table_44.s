.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global BattleCamera_SetViewFar
	.thumb_func
BattleCamera_SetViewFar:
	overlay_veneer BattleCamera_SetView
	.global BattleCamera_SetViewAtPairMidpointFar
	.thumb_func
BattleCamera_SetViewAtPairMidpointFar:
	overlay_veneer BattleCamera_SetViewAtPairMidpoint
	.global BattleStaging_ConfigurePaletteFadeFar
	.thumb_func
BattleStaging_ConfigurePaletteFadeFar:
	overlay_veneer Func_0812628c
	.global BattleBackground_LoadFar
	.thumb_func
BattleBackground_LoadFar:
	overlay_veneer Func_081263fc
	.global BattleBackground_SetPaletteLevelFar
	.thumb_func
BattleBackground_SetPaletteLevelFar:
	overlay_veneer BattleBackground_SetPaletteLevel
	.global BattleClass_GetEffectHeightFar
	.thumb_func
BattleClass_GetEffectHeightFar:
	overlay_veneer BattleClass_GetEffectHeight
