@ Far-call stub table at 0x08038368: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08038368
	.thumb_func
Func_08038368:
	overlay_veneer Func_0804d968
	overlay_veneer Menu_SelectEntry19To1c
	.global Menu_RunConfirmSelectionFar
	.thumb_func
Menu_RunConfirmSelectionFar:
	overlay_veneer Menu_RunConfirmSelection
	.global Func_08038380
	.thumb_func
Func_08038380:
	overlay_veneer Func_0804da8c
	overlay_veneer Func_08044a54
