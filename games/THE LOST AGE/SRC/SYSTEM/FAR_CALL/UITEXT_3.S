@ Far-call stub table at 0x080380a8: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global UiText_DrawNumberAtOffsetFar
	.thumb_func
UiText_DrawNumberAtOffsetFar:
	overlay_veneer UiText_DrawNumberAtOffset
	.global UiText_DrawNumberInWindowFar
	.thumb_func
UiText_DrawNumberInWindowFar:
	overlay_veneer UiText_DrawNumberInWindow
	.global Func_080380b8
	.thumb_func
Func_080380b8:
	overlay_veneer Func_08041f70
	.global Func_080380c0
	.thumb_func
Func_080380c0:
	overlay_veneer Func_08041f90
