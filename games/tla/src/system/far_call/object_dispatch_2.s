.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Actor_StartScriptFar
	.thumb_func
Actor_StartScriptFar:
	overlay_veneer Actor_StartScript
	.global Func_080200b0
	.thumb_func
Func_080200b0:
	overlay_veneer Func_08023524
	.global Actor_FindFreeObjectFar
	.thumb_func
Actor_FindFreeObjectFar:
	overlay_veneer Actor_FindFreeObject
