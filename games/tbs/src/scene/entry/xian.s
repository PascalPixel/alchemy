@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Xian_Initialize
	overlay_veneer Xian_GetEntrances
	overlay_veneer Xian_GetExits
	overlay_veneer Xian_GetPlacements
	overlay_veneer Xian_GetEvents
	overlay_veneer Xian_GetRegions
