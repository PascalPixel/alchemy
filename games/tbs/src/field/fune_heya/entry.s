@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer FuneHeya_ApplyEntryState
	overlay_veneer FuneHeya_SelectEntrances
	overlay_veneer FuneHeya_GetExits
	overlay_veneer FuneHeya_GetStepScript
	overlay_veneer SceneData_SelectTableBySceneIndexAndFlags
	overlay_veneer SceneData_ReturnZero
