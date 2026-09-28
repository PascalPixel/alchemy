@ Entry veneers opening the IMIRU_FUCHIN overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — IMIRU_FUCHIN overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer ImiruFuchin_ApplyEntryHook
	overlay_veneer SceneData_SelectTableBySceneId
	overlay_veneer ImiruFuchin_GetExits
	overlay_veneer SceneData_SelectDataByRuntimeSelector
	overlay_veneer SceneData_SelectDataByRuntimeSelectorB
	overlay_veneer SceneData_ReturnZero
