@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ValeKradensCottage_Initialize
	overlay_veneer ValeKradensCottage_GetWarpDests
	overlay_veneer ValeKradensCottage_GetWarps
	overlay_veneer ValeKradensCottage_GetObjectEventTemplates
	overlay_veneer ValeKradensCottage_GetEvents
	overlay_veneer ValeKradensCottage_GetRegions
