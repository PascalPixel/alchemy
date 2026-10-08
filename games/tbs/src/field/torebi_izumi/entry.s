.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer TorebiIzumi_OpenScene
	overlay_veneer SceneData_GetSceneTableA
	overlay_veneer SceneData_GetSceneTableB
	overlay_veneer Scene_GetPlacements
	overlay_veneer Scene_GetEvents
	overlay_veneer SceneData_ReturnZero
