.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global ObjectDispatch_StartCameraFar
	.thumb_func
ObjectDispatch_StartCameraFar:
	overlay_veneer ObjectDispatch_StartCamera
	.global Func_080200e0
	.thumb_func
Func_080200e0:
	overlay_veneer Func_08023510
