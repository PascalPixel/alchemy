.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Camera_WorldToScreenFar
	.thumb_func
Camera_WorldToScreenFar:
	overlay_veneer Camera_WorldToScreen
