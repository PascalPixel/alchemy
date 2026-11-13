.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	.global Task_Add
	.thumb_func
Task_Add:
	overlay_veneer Task_AddFar
	.global GameFlag_Set
	.thumb_func
GameFlag_Set:
	overlay_veneer GameFlag_SetFar
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
	overlay_veneer Rope_InitializeFar
	.global Func_02000198
	.thumb_func
Func_02000198:
	overlay_veneer Rope_TieFar
	.global Func_020001a0
	.thumb_func
Func_020001a0:
	overlay_veneer Rope_LashFar
	.global Func_020001a8
	.thumb_func
Func_020001a8:
	overlay_veneer Rope_PlaceFar
