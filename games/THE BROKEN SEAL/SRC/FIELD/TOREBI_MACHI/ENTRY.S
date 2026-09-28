@ Entry veneers opening the TOREBI_MACHI overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — TOREBI_MACHI overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02000728
	overlay_veneer Func_02000208
	overlay_veneer Func_02000214
	overlay_veneer Func_0200021c
	overlay_veneer Func_020004b0
	overlay_veneer Func_02000210
