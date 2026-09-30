@ Far-call stub table at 0x080200a8: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080200a8
	.thumb_func
Func_080200a8:
	.global Object_SetCallback
	.thumb_func
Object_SetCallback:
	overlay_veneer ObjectDispatch_Initialize
	.global Func_080200b0
	.thumb_func
Func_080200b0:
	overlay_veneer Func_08023524
	overlay_veneer Func_080231a4
