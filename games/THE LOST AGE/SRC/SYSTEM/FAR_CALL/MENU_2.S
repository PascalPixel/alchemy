@ Far-call stub table at 0x08038368: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Menu_SelectEntry11To14Far
	.thumb_func
Menu_SelectEntry11To14Far:
	overlay_veneer Menu_SelectEntry11To14
	.global Menu_SelectEntry19To1cFar
	.thumb_func
Menu_SelectEntry19To1cFar:
	overlay_veneer Menu_SelectEntry19To1c
	.global Menu_RunConfirmSelectionFar
	.thumb_func
Menu_RunConfirmSelectionFar:
	overlay_veneer Menu_RunConfirmSelection
	.global Menu_SelectEntry20To21Far
	.thumb_func
Menu_SelectEntry20To21Far:
	overlay_veneer Menu_SelectEntry20To21
	.global Func_08038388
	.thumb_func
Func_08038388:
	overlay_veneer Func_08044a54
