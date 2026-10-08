.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.balign 4
	overlay_veneer ShindenHeya_ApplyEntryState
	overlay_veneer ShindenHeya_GetEntrances
	overlay_veneer ShindenHeya_GetExits
	overlay_veneer SceneData_SelectPlacementTableBySubstate
	overlay_veneer SceneData_SelectTableBySceneIdAndFlags
	overlay_veneer SceneData_ReturnZero
