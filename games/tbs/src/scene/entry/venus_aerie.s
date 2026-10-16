@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer VenusAerie_Initialize
	overlay_veneer VenusAerie_GetEntrances
	overlay_veneer VenusAerie_GetExits
	overlay_veneer VenusAerie_GetPlacements
	overlay_veneer VenusAerie_GetEvents
	overlay_veneer VenusAerie_GetRegions
