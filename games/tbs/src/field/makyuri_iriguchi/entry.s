.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer MakyuriIriguchi_ApplyEntryState
	overlay_veneer SceneData_GetSceneTableA
	overlay_veneer SceneData_GetSceneTableB
	overlay_veneer SceneData_GetSceneTableC
	overlay_veneer SceneData_GetSceneTableD
	overlay_veneer SceneData_ReturnZero
