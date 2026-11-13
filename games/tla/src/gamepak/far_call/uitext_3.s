.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Text_DrawNumberAtOffsetFar
	.thumb_func
Text_DrawNumberAtOffsetFar:
	overlay_veneer Text_DrawNumberAtOffset
	.global Text_DrawNumberInWindowFar
	.thumb_func
Text_DrawNumberInWindowFar:
	overlay_veneer Text_DrawNumberInWindow
	.global Text_SetColorFar
	.thumb_func
Text_SetColorFar:
	overlay_veneer Text_SetColor
	.global Text_SetStyleFar
	.thumb_func
Text_SetStyleFar:
	overlay_veneer Text_SetStyle
