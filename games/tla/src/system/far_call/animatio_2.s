.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Animation_SetIndexAndInitObjectsFar
	.thumb_func
Animation_SetIndexAndInitObjectsFar:
	overlay_veneer Animation_SetIndexAndInitObjects
	.global Func_08020240
	.thumb_func
Func_08020240:
	overlay_veneer Func_080237f8
	.global Func_08020248
	.thumb_func
Func_08020248:
	overlay_veneer Func_08023840
	.global Animation_SetDisplayFlagFar
	.thumb_func
Animation_SetDisplayFlagFar:
	overlay_veneer Animation_SetDisplayFlag
