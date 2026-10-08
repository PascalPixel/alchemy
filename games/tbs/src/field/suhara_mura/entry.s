.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer SceneState_InitEntryWorkspaceAndFlag96f
	overlay_veneer SceneData_GetScriptTable
	overlay_veneer SceneData_GetMessageTable
	overlay_veneer SceneData_SelectActorTableByFlag96f
	overlay_veneer SceneData_SelectExtraTableByFlag96f
	overlay_veneer SceneData_ReturnZero
