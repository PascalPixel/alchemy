@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Kolima_Initialize
	overlay_veneer Kolima_GetEntrances
	overlay_veneer Kolima_GetExits
	overlay_veneer Kolima_GetPlacements
	overlay_veneer Kolima_GetEvents
	overlay_veneer Kolima_GetRegions
