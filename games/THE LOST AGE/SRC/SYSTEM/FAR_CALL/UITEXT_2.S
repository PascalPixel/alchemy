@ Far-call stub table at 0x08038090: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global UiText_DrawStringAtOffsetFar
	.thumb_func
UiText_DrawStringAtOffsetFar:
	overlay_veneer UiText_DrawStringAtOffset
	.global UiText_DrawStringInWindowFar
	.thumb_func
UiText_DrawStringInWindowFar:
	overlay_veneer UiText_DrawStringInWindow
	.global UiText_DrawNumberFar
	.thumb_func
UiText_DrawNumberFar:
	overlay_veneer UiText_DrawNumber
