@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer SolStatue_Initialize
	overlay_veneer SolStatue_GetEntrances
	overlay_veneer SolStatue_GetExits
	overlay_veneer SolStatue_GetPlacements
	overlay_veneer SolStatue_GetEvents
	overlay_veneer SolStatue_GetRegions
