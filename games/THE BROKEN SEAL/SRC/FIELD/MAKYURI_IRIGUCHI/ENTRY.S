@ Entry veneers opening the MAKYURI_IRIGUCHI overlay at its load address:
@ fixed 8-byte veneers through which the main image calls into the overlay,
@ each loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — MAKYURI_IRIGUCHI overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_020012e0
	overlay_veneer Func_02000e90
	overlay_veneer Func_02000e9c
	overlay_veneer Func_02000ea4
	overlay_veneer Func_02000fd0
	overlay_veneer Func_02000e98
