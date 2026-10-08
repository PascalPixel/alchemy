.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global UiText_DrawNumberAtOffsetFar
	.thumb_func
UiText_DrawNumberAtOffsetFar:
	overlay_veneer UiText_DrawNumberAtOffset
	.global UiText_DrawNumberInWindowFar
	.thumb_func
UiText_DrawNumberInWindowFar:
	overlay_veneer UiText_DrawNumberInWindow
	.global UiWork_SetParamNibbleFar
	.thumb_func
UiWork_SetParamNibbleFar:
	overlay_veneer Func_08041f70
	.global Func_080380c0
	.thumb_func
Func_080380c0:
	overlay_veneer Func_08041f90
