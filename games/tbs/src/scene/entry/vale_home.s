@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ValeHome_Initialize
	overlay_veneer ValeHome_GetEntrances
	overlay_veneer ValeHome_GetExits
	overlay_veneer ValeHome_GetPlacements
	overlay_veneer ValeHome_GetEvents
	overlay_veneer ValeHome_GetRegions
