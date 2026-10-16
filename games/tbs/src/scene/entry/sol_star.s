@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer SolStar_Initialize
	overlay_veneer SolStar_GetEntrances
	overlay_veneer SolStar_GetExits
	overlay_veneer SolStar_GetPlacements
	overlay_veneer SolStar_GetEvents
	overlay_veneer SolStar_GetRegions
