@ Entry veneers opening the FUNE_HEYA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — FUNE_HEYA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer FuneHeya_ApplyEntryState
	overlay_veneer FuneHeya_SelectEntrances
	overlay_veneer FuneHeya_GetExits
	overlay_veneer FuneHeya_GetStepScript
	overlay_veneer SceneData_SelectTableBySceneIndexAndFlags
	overlay_veneer SceneData_ReturnZero
