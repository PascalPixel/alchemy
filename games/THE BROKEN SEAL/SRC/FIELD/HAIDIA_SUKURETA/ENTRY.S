.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer HaidiaSukureta_RestoreEntryState
	overlay_veneer SceneData_GetScriptTable
	overlay_veneer SceneData_GetMessageTable
	overlay_veneer SceneData_GetActorTable
	overlay_veneer SceneData_SelectEffectTable
	overlay_veneer SceneData_ReturnZero
