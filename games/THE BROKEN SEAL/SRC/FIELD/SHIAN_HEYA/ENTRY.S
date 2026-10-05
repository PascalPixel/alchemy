@ Entry veneers opening the SHIAN_HEYA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — SHIAN_HEYA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer ShianHeya_PrepareWindowTransition
	overlay_veneer ShianHeya_GetEntrances
	overlay_veneer ShianHeya_GetExits
	overlay_veneer ShianHeya_SelectPlacements
	overlay_veneer ShianHeya_SelectEvents
	overlay_veneer SceneData_ReturnZero
