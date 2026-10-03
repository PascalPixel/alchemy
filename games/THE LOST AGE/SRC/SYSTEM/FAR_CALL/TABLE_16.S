@ Far-call stub table at 0x08038200: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08038200
	.thumb_func
Func_08038200:
	overlay_veneer Func_0803f86c
	.global UiTimedNotice_CloseIfActiveFar
	.thumb_func
UiTimedNotice_CloseIfActiveFar:
	overlay_veneer UiTimedNotice_CloseIfActive
	.global Func_08038210
	.thumb_func
Func_08038210:
	overlay_veneer Func_0803ce64
