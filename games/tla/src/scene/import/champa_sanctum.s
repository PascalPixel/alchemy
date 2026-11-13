.syntax unified
	.thumb
	.include "games/tla/src/system/overlay/imports.inc"
	.balign 4
	.global Actor_GetById
	.thumb_func
Actor_GetById:
	overlay_veneer Actor_GetByIdFar
	.global Actor_SetSpritePriority
	.thumb_func
Actor_SetSpritePriority:
	overlay_veneer Actor_SetSpritePriorityFar
	.global Sanctum_Run
	.thumb_func
Sanctum_Run:
	overlay_veneer Sanctum_RunFar
