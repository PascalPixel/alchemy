.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Func_08020370
	.thumb_func
Func_08020370:
	overlay_veneer Func_080219cc
	.global Func_08020378
	.thumb_func
Func_08020378:
	overlay_veneer Map_LoadCellsAndShapes
