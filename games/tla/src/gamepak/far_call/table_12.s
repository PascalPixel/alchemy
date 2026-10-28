.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global VramBlock_Reset93Far
	.thumb_func
VramBlock_Reset93Far:
	overlay_veneer VramBlock_Reset93
	.global Func_08020388
	.thumb_func
Func_08020388:
	overlay_veneer VramBlock_Load93
	.global Func_08020390
	.thumb_func
Func_08020390:
	overlay_veneer Func_0802a650
	.global Func_08020398
	.thumb_func
Func_08020398:
	overlay_veneer Func_0802de8c
