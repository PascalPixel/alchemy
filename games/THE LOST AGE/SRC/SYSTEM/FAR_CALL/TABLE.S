@ Far-call stub table at 0x08020010: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08020010
	.thumb_func
Func_08020010:
	overlay_veneer Func_080220f0
	.global Func_08020018
	.thumb_func
Func_08020018:
	overlay_veneer Func_08022318
	.global Func_08020020
	.thumb_func
Func_08020020:
	overlay_veneer Func_0802254c
	.global InitializeAnimationObjectsFar
	.thumb_func
InitializeAnimationObjectsFar:
	overlay_veneer InitializeAnimationObjects
