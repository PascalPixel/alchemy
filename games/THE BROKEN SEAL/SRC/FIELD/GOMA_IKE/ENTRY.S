.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Scene_PlaceActor8OnGate300
	overlay_veneer SceneData_GetPrimaryTable
	overlay_veneer SceneData_GetSecondaryTable
	overlay_veneer SceneData_GetTertiaryTable
	overlay_veneer SceneData_GetQuaternaryTable
	overlay_veneer SceneData_ReturnZero
