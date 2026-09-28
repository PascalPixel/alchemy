@ Entry veneers opening the SHIAN_MURA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — SHIAN_MURA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer ShianMura_SetupScene
	overlay_veneer Func_0200059c
	overlay_veneer Func_020005a8
	overlay_veneer Func_020005b0
	overlay_veneer Func_02001140
	overlay_veneer Func_020005a4
