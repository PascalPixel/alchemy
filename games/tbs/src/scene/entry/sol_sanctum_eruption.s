@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer SolSanctumEruption_Initialize
	overlay_veneer SolSanctumEruption_GetEntrances
	overlay_veneer SolSanctumEruption_GetExits
	overlay_veneer SolSanctumEruption_GetPlacements
	overlay_veneer SolSanctumEruption_GetEvents
	overlay_veneer SolSanctumEruption_GetRegions
