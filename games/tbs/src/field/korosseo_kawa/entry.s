@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer KorosseoKawa_ApplyEntryState
	overlay_veneer KorosseoKawa_GetEntrances
	overlay_veneer KorosseoKawa_GetExits
	overlay_veneer KorosseoKawa_GetPlacements
	overlay_veneer KorosseoKawa_GetEvents
	overlay_veneer get_default_result
