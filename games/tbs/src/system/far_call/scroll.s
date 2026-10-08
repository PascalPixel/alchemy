.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.global FarCall_ScrollTable
FarCall_ScrollTable:
	.global DisplayScroll_RunSlideshowFar
	.thumb_func
DisplayScroll_RunSlideshowFar:
	overlay_veneer DisplayScroll_RunSlideshow
