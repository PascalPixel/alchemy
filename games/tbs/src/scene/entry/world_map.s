@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer WorldMap_Initialize
	overlay_veneer WorldMap_GetEntrances
	overlay_veneer WorldMap_GetExits
	overlay_veneer WorldMap_GetPlacements
	overlay_veneer WorldMap_GetEvents
	overlay_veneer WorldMap_GetRegions
