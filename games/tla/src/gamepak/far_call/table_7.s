.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Func_08020308
	.thumb_func
Func_08020308:
	overlay_veneer Func_0802d7b0
	.global Func_08020310
	.thumb_func
Func_08020310:
	overlay_veneer Func_0802d71c
	.global Func_08020318
	.thumb_func
Func_08020318:
	overlay_veneer Func_0802db64
	.global Func_08020320
	.thumb_func
Func_08020320:
	overlay_veneer Func_0802db88
