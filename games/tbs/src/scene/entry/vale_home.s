@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ValeHome_RestoreEntryState
	overlay_veneer ValeHome_GetEntrances
	overlay_veneer ValeHome_SelectExits
	overlay_veneer ValeHome_SelectPlacements
	overlay_veneer ValeHome_SelectEvents
	overlay_veneer ValeHome_GetDefaultResult
