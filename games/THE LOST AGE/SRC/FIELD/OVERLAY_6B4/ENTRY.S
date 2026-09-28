@ Entry veneers opening the OVERLAY_6B4 overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — OVERLAY_6B4 overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02000564
	overlay_veneer Func_02000280
	overlay_veneer Func_0200028c
	overlay_veneer Func_02000294
	overlay_veneer Func_0200029c
	overlay_veneer Func_02000288
	overlay_veneer Func_02001408
