.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.balign 4
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
	.global GameFlag_SetBit
	.thumb_func
GameFlag_SetBit:
	overlay_veneer GameFlag_SetBitFar
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetModeById
	.thumb_func
Actor_SetModeById:
	overlay_veneer Actor_SetModeByIdFar
	.global Func_02000190
	.thumb_func
Func_02000190:
	overlay_veneer Func_080c86a8
	.global Func_02000198
	.thumb_func
Func_02000198:
	overlay_veneer Func_080c86b0
	.global Func_020001a0
	.thumb_func
Func_020001a0:
	overlay_veneer Func_080c86c0
	.global Func_020001a8
	.thumb_func
Func_020001a8:
	overlay_veneer Func_080c86e8
