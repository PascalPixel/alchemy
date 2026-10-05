.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global FarCall_ScrollTable
FarCall_ScrollTable:
	.global Func_080f0000
	.thumb_func
Func_080f0000:
	overlay_veneer DisplayScroll_RunSlideshow