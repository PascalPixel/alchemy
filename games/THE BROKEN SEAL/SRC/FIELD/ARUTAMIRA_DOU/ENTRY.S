@ Entry veneers opening the ARUTAMIRA_DOU overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — ARUTAMIRA_DOU overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer ArutamiraDou_ApplyEntryState
	overlay_veneer ArutamiraDou_GetEntrances
	overlay_veneer ArutamiraDou_GetExits
	overlay_veneer Scene_GetPlacements
	overlay_veneer Scene_GetEvents
	overlay_veneer SceneData_ReturnZero
