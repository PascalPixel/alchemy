@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ValeInteriorsShops_Initialize
	overlay_veneer ValeInteriorsShops_GetWarpDests
	overlay_veneer ValeInteriorsShops_GetWarps
	overlay_veneer ValeInteriorsShops_GetObjectEventTemplates
	overlay_veneer ValeInteriorsShops_GetEvents
	overlay_veneer ValeInteriorsShops_GetRegions
