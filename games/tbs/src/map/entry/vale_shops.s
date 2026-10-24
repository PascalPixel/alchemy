@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ValeShops_Initialize
	overlay_veneer ValeShops_GetWarpDests
	overlay_veneer ValeShops_GetWarps
	overlay_veneer ValeShops_GetObjectEventTemplates
	overlay_veneer ValeShops_GetEvents
	overlay_veneer ValeShops_GetRegions
