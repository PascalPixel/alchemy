@ Far-call stub table at 0x08020328: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08020328
	.thumb_func
Func_08020328:
	overlay_veneer Func_0802b7e0
	overlay_veneer Func_080237c8
	overlay_veneer Func_08022f24
