@ Entry veneers opening the HAIDIA_SUKURETA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — HAIDIA_SUKURETA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_0200150c
	overlay_veneer Func_02000088
	overlay_veneer Func_02000094
	overlay_veneer Func_0200009c
	overlay_veneer Func_020000a4
	overlay_veneer Func_02000090
