@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer SolEruption_Initialize
	overlay_veneer SolEruption_GetEntrances
	overlay_veneer SolEruption_GetExits
	overlay_veneer SolEruption_GetPlacements
	overlay_veneer SolEruption_GetEvents
	overlay_veneer SolEruption_GetRegions
