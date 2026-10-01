@ Far-call stub table at 0x080200c0: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080200c0
	.thumb_func
Func_080200c0:
	overlay_veneer Func_08023220
	.global Object_Destroy
	.thumb_func
Object_Destroy:
	overlay_veneer Func_080231c8
	.global ObjectDispatch_SetChildField12Far
	.thumb_func
ObjectDispatch_SetChildField12Far:
	overlay_veneer ObjectDispatch_SetChildField12
