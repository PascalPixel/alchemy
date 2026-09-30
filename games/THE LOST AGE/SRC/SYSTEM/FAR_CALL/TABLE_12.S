@ Far-call stub table at 0x08020380: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08020380
	.thumb_func
Func_08020380:
	overlay_veneer Func_08022c78
	overlay_veneer Func_08022c84
	.global Func_08020390
	.thumb_func
Func_08020390:
	overlay_veneer Func_0802a650
	overlay_veneer Func_0802de8c
