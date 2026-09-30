@ Far-call stub table at 0x080200e8: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080200e8
	.thumb_func
Func_080200e8:
	overlay_veneer Object_SetPositionAndResetMotion
	overlay_veneer Func_08022f64
	overlay_veneer Func_08022fe8
	overlay_veneer Func_0802301c
	overlay_veneer Func_0802a6b8
	overlay_veneer Func_0802b998
	overlay_veneer Func_0802af9c
