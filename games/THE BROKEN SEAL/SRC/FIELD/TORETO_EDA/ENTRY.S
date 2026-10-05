@ Entry veneers opening the TORETO_EDA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — TORETO_EDA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer ToretoEda_InitializeBackdropTransition
	overlay_veneer ToretoEda_GetEntrances
	overlay_veneer ToretoEda_GetExits
	overlay_veneer ToretoEda_GetPlacements
	overlay_veneer ToretoEda_GetEvents
	overlay_veneer SceneData_ReturnZero
