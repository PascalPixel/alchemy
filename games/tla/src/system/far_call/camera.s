.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Camera_WorldToScreenFar
	.thumb_func
Camera_WorldToScreenFar:
	overlay_veneer Camera_WorldToScreen
