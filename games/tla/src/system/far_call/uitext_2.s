.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Text_DrawStringAtOffsetFar
	.thumb_func
Text_DrawStringAtOffsetFar:
	overlay_veneer Text_DrawStringAtOffset
	.global Text_DrawStringInWindowFar
	.thumb_func
Text_DrawStringInWindowFar:
	overlay_veneer Text_DrawStringInWindow
	.global Text_DrawNumberFar
	.thumb_func
Text_DrawNumberFar:
	overlay_veneer Text_DrawNumber
