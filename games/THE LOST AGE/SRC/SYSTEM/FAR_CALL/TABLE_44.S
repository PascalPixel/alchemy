@ Far-call stub table at 0x08118028: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
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
	.global Func_08118050
	.thumb_func
Func_08118050:
	overlay_veneer Func_08128194
