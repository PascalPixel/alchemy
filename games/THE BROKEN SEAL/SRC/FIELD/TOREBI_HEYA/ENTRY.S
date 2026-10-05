@ Entry veneers opening the TOREBI_HEYA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — TOREBI_HEYA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Scene_InitFacingActors
	overlay_veneer TorebiHeya_GetEntrances
	overlay_veneer TorebiHeya_GetExits
	overlay_veneer TorebiHeya_SelectPlacements
	overlay_veneer TorebiHeya_SelectEvents
	overlay_veneer TorebiHeya_GetDefaultResult
