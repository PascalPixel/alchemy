@ Far-call stub table at 0x080c8378: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080c8378
	.thumb_func
Func_080c8378:
	overlay_veneer Func_080d170c
	.global Func_080c8380
	.thumb_func
Func_080c8380:
	overlay_veneer Func_080d172c
	overlay_veneer Func_080d0c50
