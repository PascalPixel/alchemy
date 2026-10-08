.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer Scene_PlaceActor8OnGate300
	overlay_veneer SceneData_GetPrimaryTable
	overlay_veneer SceneData_GetSecondaryTable
	overlay_veneer SceneData_GetTertiaryTable
	overlay_veneer SceneData_GetQuaternaryTable
	overlay_veneer SceneData_ReturnZero
