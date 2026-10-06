.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer MakyuriIriguchi_ApplyEntryState
	overlay_veneer SceneData_GetSceneTableA
	overlay_veneer SceneData_GetSceneTableB
	overlay_veneer SceneData_GetSceneTableC
	overlay_veneer SceneData_GetSceneTableD
	overlay_veneer SceneData_ReturnZero
