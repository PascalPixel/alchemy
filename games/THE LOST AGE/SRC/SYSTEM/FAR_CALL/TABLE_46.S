@ Far-call stub table at 0x08118070: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Battle_GetObjectTableValueFar
	.thumb_func
Battle_GetObjectTableValueFar:
	overlay_veneer Func_0811c37c
	.global BattleMotion_ApproachTargetFar
	.thumb_func
BattleMotion_ApproachTargetFar:
	overlay_veneer Func_0811c120
	.global Func_08118080
	.thumb_func
Func_08118080:
	overlay_veneer Func_0811c01c
