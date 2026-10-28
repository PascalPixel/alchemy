.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Animation_SetWorkEntryFar
	.thumb_func
Animation_SetWorkEntryFar:
	overlay_veneer Animation_SetWorkEntry
	.global ObjectSystem_InitializeFar
	.thumb_func
ObjectSystem_InitializeFar:
	overlay_veneer Func_080230e0
	.global Actor_SetMode
	.thumb_func
Actor_SetMode:
	overlay_veneer Actor_SelectAnimation
	.global Actor_SetAnimationSpeedFar
	.thumb_func
Actor_SetAnimationSpeedFar:
	overlay_veneer Actor_SetAnimationSpeed
	.global Actor_SetAnimationAndSpeedFar
	.thumb_func
Actor_SetAnimationAndSpeedFar:
	overlay_veneer Actor_SetAnimationAndSpeed
