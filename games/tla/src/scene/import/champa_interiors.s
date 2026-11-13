.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	.global GameFlag_Test
	.thumb_func
GameFlag_Test:
	overlay_veneer GameFlag_TestFar
	.global GameFlag_ClearBit
	.thumb_func
GameFlag_ClearBit:
	overlay_veneer GameFlag_ClearBitFar
	.global Shop_Run
	.thumb_func
Shop_Run:
	overlay_veneer Func_08108008
	.global Inn_CheckIn
	.thumb_func
Inn_CheckIn:
	overlay_veneer Func_08108018
