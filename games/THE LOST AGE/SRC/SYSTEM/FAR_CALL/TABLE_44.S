.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global BattlePres_SetupTransitionSceneFar
	.thumb_func
BattlePres_SetupTransitionSceneFar:
	overlay_veneer BattlePres_SetupTransitionScene
	.global BattlePres_SetupTransitionAtPairMidpointFar
	.thumb_func
BattlePres_SetupTransitionAtPairMidpointFar:
	overlay_veneer BattlePres_SetupTransitionAtPairMidpoint
	.global Func_08118038
	.thumb_func
Func_08118038:
	overlay_veneer Func_0812628c
	.global Func_08118040
	.thumb_func
Func_08118040:
	overlay_veneer Func_081263fc
	.global Func_08118048
	.thumb_func
Func_08118048:
	overlay_veneer Func_08126218
	.global Summon_GetEntryByte4Far
	.thumb_func
Summon_GetEntryByte4Far:
	overlay_veneer Summon_GetEntryByte4
