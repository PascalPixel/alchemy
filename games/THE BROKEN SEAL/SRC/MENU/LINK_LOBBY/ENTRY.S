@ Entry veneers opening the LINK_LOBBY overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — LINK_LOBBY overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_020012e0
	overlay_veneer Func_02000030
	overlay_veneer Func_02000058
	overlay_veneer Func_02000060
	overlay_veneer Func_020011b4
	overlay_veneer Func_02000038
