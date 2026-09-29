@ Entry veneers opening the ITEM_LEVEL overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — ITEM_LEVEL overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer FieldScene_RunEntrySetup
	overlay_veneer ItemLevel_GetEntrances
	overlay_veneer ItemLevel_GetExits
	overlay_veneer ItemLevel_GetPlacements
	overlay_veneer ItemLevel_GetEvents
	overlay_veneer SceneData_ReturnZero
