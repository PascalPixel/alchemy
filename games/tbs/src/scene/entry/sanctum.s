@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Sanctum_ApplyEntryState
	overlay_veneer Sanctum_GetEntrances
	overlay_veneer Sanctum_GetExits
	overlay_veneer SceneData_SelectPlacementTableBySubstate
	overlay_veneer SceneData_SelectTableBySceneIdAndFlags
	overlay_veneer SceneData_ReturnZero
