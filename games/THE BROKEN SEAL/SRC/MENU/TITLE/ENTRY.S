@ Entry veneers opening the TITLE overlay at its load address: fixed 8-byte
@ veneers through which the main image calls into the overlay, each loading
@ its target into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — TITLE overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Title_Run
	overlay_veneer Title_GetEntrances
	overlay_veneer Title_GetExits
	overlay_veneer Title_GetPlacements
	overlay_veneer Title_GetEvents
	overlay_veneer SceneData_ReturnZero
