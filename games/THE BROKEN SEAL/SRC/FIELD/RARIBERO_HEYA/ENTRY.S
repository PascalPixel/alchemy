@ Entry veneers opening the RARIBERO_HEYA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — RARIBERO_HEYA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Func_0200104c
	overlay_veneer RariberoHeya_GetEntrances
	overlay_veneer RariberoHeya_GetExits
	overlay_veneer Func_02000084
	overlay_veneer Func_020004b0
	overlay_veneer Func_0200004c
