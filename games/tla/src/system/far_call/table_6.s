.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Map_SetScreenShakeFar
	.thumb_func
Map_SetScreenShakeFar:
	overlay_veneer Map_SetScreenShake
	.global Func_08020230
	.thumb_func
Func_08020230:
	overlay_veneer Map_WaitForShakeToSettle
