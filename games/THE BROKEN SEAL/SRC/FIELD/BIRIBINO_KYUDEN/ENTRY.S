@ Entry veneers opening the BIRIBINO_KYUDEN overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — BIRIBINO_KYUDEN overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_020006e0
	overlay_veneer Func_02000088
	overlay_veneer Func_02000094
	overlay_veneer Func_0200009c
	overlay_veneer Func_02000100
	overlay_veneer Func_02000090
