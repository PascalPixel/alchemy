@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ValeBabi_RestoreEntryState
	overlay_veneer ValeBabi_GetEntrances
	overlay_veneer ValeBabi_SelectExits
	overlay_veneer ValeBabi_SelectPlacements
	overlay_veneer ValeBabi_SelectEvents
	overlay_veneer SceneData_ReturnZero
