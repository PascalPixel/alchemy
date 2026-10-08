.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer SuharaSabaku_RunSceneScript
	overlay_veneer Scene_GetEntrances
	overlay_veneer SuharaSabaku_GetExits
	overlay_veneer Scene_GetPlacements
	overlay_veneer Scene_GetEvents
	overlay_veneer SceneData_ReturnZero
