.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Animation_SetWorkEntryFar
	.thumb_func
Animation_SetWorkEntryFar:
	overlay_veneer Animation_SetWorkEntry
	.global ObjectSystem_InitializeFar
	.thumb_func
ObjectSystem_InitializeFar:
	overlay_veneer Func_080230e0
	.global Object_SetMode
	.thumb_func
Object_SetMode:
	overlay_veneer ObjectDispatch_SelectAnimation
	.global ObjectDispatch_SetAnimationSpeedFar
	.thumb_func
ObjectDispatch_SetAnimationSpeedFar:
	overlay_veneer ObjectDispatch_SetAnimationSpeed
	.global ObjectDispatch_SetAnimationAndSpeedFar
	.thumb_func
ObjectDispatch_SetAnimationAndSpeedFar:
	overlay_veneer ObjectDispatch_SetAnimationAndSpeed
