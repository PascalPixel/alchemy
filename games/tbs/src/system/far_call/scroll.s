@ C2: Every entry uses overlay_veneer, an aligned 8-byte Thumb ldr r4/bx
@ r4/target-word stub.
.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.global FarCall_ScrollTable
FarCall_ScrollTable:
	.global DisplayScroll_RunSlideshowFar
	.thumb_func
DisplayScroll_RunSlideshowFar:
	overlay_veneer DisplayScroll_RunSlideshow
