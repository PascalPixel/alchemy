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
	overlay_veneer Func_08041f70
	.global Func_080380c0
	.thumb_func
Func_080380c0:
	overlay_veneer Func_08041f90
