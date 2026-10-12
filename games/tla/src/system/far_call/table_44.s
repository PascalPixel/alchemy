.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global BattlePres_SetupTransitionSceneFar
	.thumb_func
BattlePres_SetupTransitionSceneFar:
	overlay_veneer BattlePres_SetupTransitionScene
	.global BattlePres_SetupTransitionAtPairMidpointFar
	.thumb_func
BattlePres_SetupTransitionAtPairMidpointFar:
	overlay_veneer BattlePres_SetupTransitionAtPairMidpoint
	.global BattlePresentation_ConfigurePaletteFadeFar
	.thumb_func
BattlePresentation_ConfigurePaletteFadeFar:
	overlay_veneer Func_0812628c
	.global BattleBackground_LoadFar
	.thumb_func
BattleBackground_LoadFar:
	overlay_veneer Func_081263fc
	.global BattlePresentation_SetPaletteLevelFar
	.thumb_func
BattlePresentation_SetPaletteLevelFar:
	overlay_veneer BattlePresentation_SetPaletteLevel
	.global Summon_GetEntryByte4Far
	.thumb_func
Summon_GetEntryByte4Far:
	overlay_veneer Summon_GetEntryByte4
