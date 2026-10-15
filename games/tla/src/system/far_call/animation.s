.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Animation_ApplyChildArgumentFar
	.thumb_func
Animation_ApplyChildArgumentFar:
	overlay_veneer Animation_ApplyChildArgument
	.global Func_08020038
	.thumb_func
Func_08020038:
	overlay_veneer Func_08022b80
