@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ValeInn_Initialize
	overlay_veneer ValeInn_GetWarpDests
	overlay_veneer ValeInn_GetWarps
	overlay_veneer ValeInn_GetObjectEventTemplates
	overlay_veneer ValeInn_GetEvents
	overlay_veneer ValeInn_GetRegions
