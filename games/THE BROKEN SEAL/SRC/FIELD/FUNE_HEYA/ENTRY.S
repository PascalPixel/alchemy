.syntax unified
	.thumb
	.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer FuneHeya_ApplyEntryState
	overlay_veneer FuneHeya_SelectEntrances
	overlay_veneer FuneHeya_GetExits
	overlay_veneer FuneHeya_GetStepScript
	overlay_veneer SceneData_SelectTableBySceneIndexAndFlags
	overlay_veneer SceneData_ReturnZero
