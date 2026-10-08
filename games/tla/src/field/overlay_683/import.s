.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearBitFar
	.global Func_02000178
	.thumb_func
Func_02000178:
	overlay_veneer Func_08108008
	.global Func_02000180
	.thumb_func
Func_02000180:
	overlay_veneer Func_08108018
