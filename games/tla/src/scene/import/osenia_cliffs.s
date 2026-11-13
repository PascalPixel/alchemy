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
	.global Rope_Initialize
	.thumb_func
Rope_Initialize:
	overlay_veneer Rope_InitializeFar
	.global Rope_Tie
	.thumb_func
Rope_Tie:
	overlay_veneer Rope_TieFar
	.global Rope_Lash
	.thumb_func
Rope_Lash:
	overlay_veneer Rope_LashFar
	.global Rope_Place
	.thumb_func
Rope_Place:
	overlay_veneer Rope_PlaceFar
