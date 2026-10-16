@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer CrossboneIslePillarRooms_Initialize
	overlay_veneer CrossboneIslePillarRooms_GetEntrances
	overlay_veneer CrossboneIslePillarRooms_GetExits
	overlay_veneer CrossboneIslePillarRooms_GetPlacements
	overlay_veneer CrossboneIslePillarRooms_GetEvents
	overlay_veneer CrossboneIslePillarRooms_GetRegions
