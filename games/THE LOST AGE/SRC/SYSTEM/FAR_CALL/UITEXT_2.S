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
