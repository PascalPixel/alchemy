.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global FarCall_ScrollTable
FarCall_ScrollTable:
	.global DisplayScroll_RunSlideshowFar
	.thumb_func
DisplayScroll_RunSlideshowFar:
	overlay_veneer DisplayScroll_RunSlideshow
