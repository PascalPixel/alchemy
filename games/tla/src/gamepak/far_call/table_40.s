.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Func_080f8090
	.thumb_func
Func_080f8090:
	overlay_veneer MenuIcon_SetPosition
	.global Func_080f8098
	.thumb_func
Func_080f8098:
	overlay_veneer MenuIcon_SetPicture
	.global Func_080f80a0
	.thumb_func
Func_080f80a0:
	overlay_veneer Func_08105350
