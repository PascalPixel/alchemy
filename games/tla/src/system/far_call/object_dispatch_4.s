.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Actor_StartCameraFar
	.thumb_func
Actor_StartCameraFar:
	overlay_veneer Actor_StartCamera
	.global Func_080200e0
	.thumb_func
Func_080200e0:
	overlay_veneer Func_08023510
