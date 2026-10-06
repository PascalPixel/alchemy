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
