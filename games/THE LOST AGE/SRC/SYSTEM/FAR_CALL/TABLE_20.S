@ Far-call stub table at 0x080383d0: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Ui_GetTableWordZeroFar
	.thumb_func
Ui_GetTableWordZeroFar:
	overlay_veneer Ui_GetTableWordZero
	.global Func_080383d8
	.thumb_func
Func_080383d8:
	overlay_veneer Func_0803a60c
	.global Func_080383e0
	.thumb_func
Func_080383e0:
	overlay_veneer Func_0804325c
