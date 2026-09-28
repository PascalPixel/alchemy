@ Entry veneers opening the BABI_IRIGUCHI overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — BABI_IRIGUCHI overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_020028a0
	overlay_veneer Func_02000f58
	overlay_veneer Func_02000fd4
	overlay_veneer Func_02000fdc
	overlay_veneer Func_020027a0
	overlay_veneer Func_02000fac
