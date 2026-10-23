@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer SolSanctumEntrance_Initialize
	overlay_veneer SolSanctumEntrance_GetEntrances
	overlay_veneer SolSanctumEntrance_GetExits
	overlay_veneer SolSanctumEntrance_GetPlacements
	overlay_veneer SolSanctumEntrance_GetEvents
	overlay_veneer SolSanctumEntrance_GetRegions
