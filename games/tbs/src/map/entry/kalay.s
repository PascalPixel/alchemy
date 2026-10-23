@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Kalay_Initialize
	overlay_veneer Kalay_GetEntrances
	overlay_veneer Kalay_GetExits
	overlay_veneer Kalay_GetPlacements
	overlay_veneer Kalay_GetEvents
	overlay_veneer Kalay_GetRegions
