@ Entry veneers opening the KORIMA_MURA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — KORIMA_MURA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02000694
	overlay_veneer Func_0200016c
	overlay_veneer Func_020001d4
	overlay_veneer Func_020001dc
	overlay_veneer Func_02000284
	overlay_veneer Func_020001ac
