@ Far-call stub table at 0x08020308: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08020308
	.thumb_func
Func_08020308:
	overlay_veneer Func_0802d7b0
	.global Func_08020310
	.thumb_func
Func_08020310:
	overlay_veneer Func_0802d71c
	.global Func_08020318
	.thumb_func
Func_08020318:
	overlay_veneer Func_0802db64
	.global Func_08020320
	.thumb_func
Func_08020320:
	overlay_veneer Func_0802db88
