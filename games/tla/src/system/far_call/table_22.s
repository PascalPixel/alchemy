.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Func_080ad0b0
	.thumb_func
Func_080ad0b0:
	overlay_veneer Func_080afb80
	.global Func_080ad0b8
	.thumb_func
Func_080ad0b8:
	overlay_veneer Func_080ad338
