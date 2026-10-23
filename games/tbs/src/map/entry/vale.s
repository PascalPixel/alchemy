@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Vale_Initialize
	overlay_veneer Vale_GetEntrances
	overlay_veneer Vale_GetExits
	overlay_veneer Vale_GetPlacements
	overlay_veneer Vale_GetEvents
	overlay_veneer Vale_GetRegions
