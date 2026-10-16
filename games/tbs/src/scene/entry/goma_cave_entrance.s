@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer GomaCaveEntrance_RestoreEntryState
	overlay_veneer GomaCaveEntrance_GetEntrances
	overlay_veneer GomaCaveEntrance_GetExits
	overlay_veneer GomaCaveEntrance_GetPlacements
	overlay_veneer GomaCaveEntrance_GetEvents
	overlay_veneer SceneData_ReturnZero
