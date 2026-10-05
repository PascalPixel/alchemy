.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer SoruFunka_RunHostageSceneOnEntranceTen
	overlay_veneer SceneData_GetScriptTable
	overlay_veneer SceneData_GetMessageTable
	overlay_veneer SceneData_GetActorTable
	overlay_veneer SceneData_GetEffectTable
	overlay_veneer SceneData_ReturnZero
