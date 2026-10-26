.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Func_08020328
	.thumb_func
Func_08020328:
	overlay_veneer Map_GetHeaderSize
	.global Func_08020330
	.thumb_func
Func_08020330:
	overlay_veneer Func_080237c8
	.global Animation_WaitForTrackEndFar
	.thumb_func
Animation_WaitForTrackEndFar:
	overlay_veneer Animation_WaitForTrackEnd
