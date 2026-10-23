@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer MercuryLighthouseAerie_Initialize
	overlay_veneer MercuryLighthouseAerie_GetEntrances
	overlay_veneer MercuryLighthouseAerie_GetExits
	overlay_veneer MercuryLighthouseAerie_GetPlacements
	overlay_veneer MercuryLighthouseAerie_GetEvents
	overlay_veneer MercuryLighthouseAerie_GetRegions
