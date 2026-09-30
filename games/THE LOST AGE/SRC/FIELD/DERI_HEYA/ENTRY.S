@ Entry veneers opening the DERI_HEYA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — DERI_HEYA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_02000a50
	overlay_veneer Scene_GetEntrances
	overlay_veneer Func_02000044
	overlay_veneer Func_02000064
	overlay_veneer Func_02000218
	overlay_veneer Scene_GetRegions
	overlay_veneer Func_02000c0c
