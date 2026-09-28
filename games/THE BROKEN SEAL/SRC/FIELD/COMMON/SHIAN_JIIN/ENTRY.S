@ Entry veneers opening the SHIAN_JIIN overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — SHIAN_JIIN overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02003e58
	overlay_veneer Func_0200034c
	overlay_veneer GetXianMessageData
	overlay_veneer Func_02000388
	overlay_veneer Func_02002574
	overlay_veneer GetXianInitialState
