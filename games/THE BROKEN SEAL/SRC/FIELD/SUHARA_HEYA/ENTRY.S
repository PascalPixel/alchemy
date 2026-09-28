@ Entry veneers opening the SUHARA_HEYA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — SUHARA_HEYA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02000a78
	overlay_veneer Func_02000030
	overlay_veneer Func_02000040
	overlay_veneer Func_02000048
	overlay_veneer Func_02000a54
	overlay_veneer Func_02000038
