@ Entry veneers opening the BIRIBINO_DOU overlay at its load address: fixed
@ 8-byte veneers through which the main image calls into the overlay, each
@ loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — BIRIBINO_DOU overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer FieldScene_DispatchByScenarioId
	overlay_veneer SceneData_SelectByRuntimeSelector
	overlay_veneer BiribinoDou_GetExits
	overlay_veneer SceneData_SelectSecondaryDataByRuntimeSelector
	overlay_veneer SceneData_SelectDataByRuntimeSelector
	overlay_veneer SceneData_ReturnZero
