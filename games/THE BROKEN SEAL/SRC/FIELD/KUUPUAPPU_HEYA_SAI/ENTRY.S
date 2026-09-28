@ Entry veneers opening the KUUPUAPPU_HEYA_SAI overlay at its load address:
@ fixed 8-byte veneers through which the main image calls into the overlay,
@ each loading its target into r4 and branching, which the calling convention
@ permits.
@ credit: reconstructed_veneer — KUUPUAPPU_HEYA_SAI overlay entry veneers
.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer FieldScene_InitSceneStateByStep
	overlay_veneer SceneData_GetScriptTable
	overlay_veneer SceneData_GetMessageTable
	overlay_veneer SceneData_GetPreparedActors
	overlay_veneer SceneData_GetEffectTable
	overlay_veneer SceneData_ReturnZero
