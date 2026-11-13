.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Func_08038358
	.thumb_func
Func_08038358:
	overlay_veneer PauseMenu_Run
	.global BgScreen_FillRectFar
	.thumb_func
BgScreen_FillRectFar:
	overlay_veneer BgScreen_FillRect
