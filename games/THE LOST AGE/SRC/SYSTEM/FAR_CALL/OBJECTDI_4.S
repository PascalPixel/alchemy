@ Far-call stub table at 0x080200d8: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global ObjectDispatch_InitFromTable4WithArgumentFar
	.thumb_func
ObjectDispatch_InitFromTable4WithArgumentFar:
	overlay_veneer ObjectDispatch_InitFromTable4WithArgument
	overlay_veneer Func_08023510
