.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Func_080c8710
	.thumb_func
Func_080c8710:
	overlay_veneer Func_080eab70
	.global Func_080c8718
	.thumb_func
Func_080c8718:
	overlay_veneer Func_080eabd0
	.global Func_080c8720
	.thumb_func
Func_080c8720:
	overlay_veneer Func_080eace8
	.global Func_080c8728
	.thumb_func
Func_080c8728:
	overlay_veneer Func_080eadfc
