@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Scene_PlaceActor8OnGate300
	overlay_veneer SceneData_GetPrimaryTable
	overlay_veneer SceneData_GetSecondaryTable
	overlay_veneer SceneData_GetTertiaryTable
	overlay_veneer SceneData_GetQuaternaryTable
	overlay_veneer SceneData_ReturnZero
