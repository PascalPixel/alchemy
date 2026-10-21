@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ValeInteriorsShops_Initialize
	overlay_veneer ValeInteriorsShops_GetEntrances
	overlay_veneer ValeInteriorsShops_GetExits
	overlay_veneer ValeInteriorsShops_GetPlacements
	overlay_veneer ValeInteriorsShops_GetEvents
	overlay_veneer ValeInteriorsShops_GetRegions
