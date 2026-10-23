@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer SolSanctumLunaSolRooms_Initialize
	overlay_veneer SolSanctumLunaSolRooms_GetEntrances
	overlay_veneer SolSanctumLunaSolRooms_GetExits
	overlay_veneer SolSanctumLunaSolRooms_GetPlacements
	overlay_veneer SolSanctumLunaSolRooms_GetEvents
	overlay_veneer SolSanctumLunaSolRooms_GetRegions
