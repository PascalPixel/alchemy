@ Far-call stub table at 0x08118070: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08118070
	.thumb_func
Func_08118070:
	overlay_veneer Func_0811c37c
	overlay_veneer Func_0811c120
	overlay_veneer Func_0811c01c
