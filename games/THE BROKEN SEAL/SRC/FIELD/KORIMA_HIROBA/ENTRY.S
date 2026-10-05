.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer FieldScene_SetupEntryActors8To11
	overlay_veneer SceneData_GetScriptTable
	overlay_veneer SceneData_GetMessageTable
	overlay_veneer SceneData_GetActorTable
	overlay_veneer SceneData_GetExtraTable
	overlay_veneer SceneData_ReturnZero
