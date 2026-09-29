@ Entry veneers opening the RARIBERO_HEYA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — RARIBERO_HEYA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Scene_Initialize
	overlay_veneer RariberoHeya_GetEntrances
	overlay_veneer RariberoHeya_GetExits
	overlay_veneer RariberoHeya_GetPlacements
	overlay_veneer RariberoHeya_GetEvents
	overlay_veneer RariberoHeya_GetRegions
