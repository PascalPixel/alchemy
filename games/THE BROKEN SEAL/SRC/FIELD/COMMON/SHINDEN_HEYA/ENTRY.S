@ Entry veneers opening the SHINDEN_HEYA overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — SHINDEN_HEYA overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer ShindenHeya_ApplyEntryState
	overlay_veneer SceneData_GetTableBaa8
	overlay_veneer SceneData_GetTableBbc8
	overlay_veneer SceneData_SelectPlacementTableBySubstate
	overlay_veneer SceneData_SelectTableBySceneIdAndFlags
	overlay_veneer SceneData_ReturnZero
