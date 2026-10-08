.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer KuupuappuMura_RestoreEntryState
	overlay_veneer SceneData_GetScriptTable
	overlay_veneer SceneData_GetMessageTable
	overlay_veneer SceneData_SelectActorTableByFlag855
	overlay_veneer KuupuappuMura_SelectEvents
	overlay_veneer SceneData_ReturnZero
