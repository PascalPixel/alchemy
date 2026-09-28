@ Entry veneers opening the SUHARA_SABAKU overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — SUHARA_SABAKU overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02000e8c
	overlay_veneer Func_0200034c
	overlay_veneer Func_020003a4
	overlay_veneer Func_020003ac
	overlay_veneer Func_02000e5c
	overlay_veneer Func_020003a0
