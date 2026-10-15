@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer SceneState_InitEntryWorkspaceAndFlag96f
	overlay_veneer SceneData_GetScriptTable
	overlay_veneer SceneData_GetMessageTable
	overlay_veneer SceneData_SelectActorTableByFlag96f
	overlay_veneer SceneData_SelectExtraTableByFlag96f
	overlay_veneer SceneData_ReturnZero
