@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Imil_Initialize
	overlay_veneer Imil_GetEntrances
	overlay_veneer Imil_GetExits
	overlay_veneer Imil_GetPlacements
	overlay_veneer Imil_GetEvents
	overlay_veneer Imil_GetRegions
