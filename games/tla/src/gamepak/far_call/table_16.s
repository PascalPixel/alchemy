.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Func_08038200
	.thumb_func
Func_08038200:
	overlay_veneer AreaNameWindow_Show
	.global AreaNameWindow_HideFar
	.thumb_func
AreaNameWindow_HideFar:
	overlay_veneer AreaNameWindow_Hide
	.global Func_08038210
	.thumb_func
Func_08038210:
	overlay_veneer Func_0803ce64
