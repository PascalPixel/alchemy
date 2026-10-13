@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer FieldScene_RunEntrySetup
	overlay_veneer ItemLevel_GetEntrances
	overlay_veneer ItemLevel_GetExits
	overlay_veneer ItemLevel_GetPlacements
	overlay_veneer ItemLevel_GetEvents
	overlay_veneer SceneData_ReturnZero
