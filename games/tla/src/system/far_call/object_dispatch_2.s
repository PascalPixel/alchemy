.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global ObjectDispatch_InitializeFar
	.thumb_func
ObjectDispatch_InitializeFar:
	overlay_veneer ObjectDispatch_Initialize
	.global Func_080200b0
	.thumb_func
Func_080200b0:
	overlay_veneer Func_08023524
	.global ObjectDispatch_FindFreeObjectFar
	.thumb_func
ObjectDispatch_FindFreeObjectFar:
	overlay_veneer ObjectDispatch_FindFreeObject
