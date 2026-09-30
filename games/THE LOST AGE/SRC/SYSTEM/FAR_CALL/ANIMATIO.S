@ Far-call stub table at 0x08020030: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08020030
	.thumb_func
Func_08020030:
	overlay_veneer Animation_ApplyChildArgument
	.global Func_08020038
	.thumb_func
Func_08020038:
	overlay_veneer Func_08022b80
