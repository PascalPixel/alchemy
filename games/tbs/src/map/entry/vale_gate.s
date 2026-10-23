@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ValeGate_Initialize
	overlay_veneer ValeGate_GetEntrances
	overlay_veneer ValeGate_GetExits
	overlay_veneer ValeGate_GetPlacements
	overlay_veneer ValeGate_GetEvents
	overlay_veneer ValeGate_GetRegions
