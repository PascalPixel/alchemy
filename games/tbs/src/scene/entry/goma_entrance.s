@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer GomaEntrance_RestoreEntryState
	overlay_veneer GomaEntrance_GetEntrances
	overlay_veneer GomaEntrance_GetExits
	overlay_veneer GomaEntrance_GetPlacements
	overlay_veneer GomaEntrance_GetEvents
	overlay_veneer SceneData_ReturnZero
