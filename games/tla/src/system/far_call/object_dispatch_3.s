.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Func_080200c0
	.thumb_func
Func_080200c0:
	overlay_veneer Func_08023220
	.global Actor_Destroy
	.thumb_func
Actor_Destroy:
	overlay_veneer Actor_Release
	.global Actor_SetSpriteRotationFar
	.thumb_func
Actor_SetSpriteRotationFar:
	overlay_veneer Actor_SetSpriteRotation
