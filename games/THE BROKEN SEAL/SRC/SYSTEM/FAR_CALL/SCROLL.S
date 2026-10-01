@ Far-call stub table heading the display scroll code at 0x080f0000: fixed 8-byte
@ veneers, each loading its target into r4 and branching, which the calling
@ convention permits. No compiler emits them; the shared overlay veneer macro
@ builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.global FarCall_ScrollTable
FarCall_ScrollTable:
	.global Func_080f0000
	.thumb_func
Func_080f0000:
	overlay_veneer DisplayScroll_RunSlideshow