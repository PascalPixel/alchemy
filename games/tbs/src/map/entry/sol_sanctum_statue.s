@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer SolSanctumStatue_Initialize
	overlay_veneer SolSanctumStatue_GetEntrances
	overlay_veneer SolSanctumStatue_GetExits
	overlay_veneer SolSanctumStatue_GetPlacements
	overlay_veneer SolSanctumStatue_GetEvents
	overlay_veneer SolSanctumStatue_GetRegions
