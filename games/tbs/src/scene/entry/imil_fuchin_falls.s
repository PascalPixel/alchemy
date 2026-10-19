@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ImilFuchinFalls_Initialize
	overlay_veneer ImilFuchinFalls_GetEntrances
	overlay_veneer ImilFuchinFalls_GetExits
	overlay_veneer ImilFuchinFalls_GetPlacements
	overlay_veneer ImilFuchinFalls_GetEvents
	overlay_veneer ImilFuchinFalls_GetRegions
