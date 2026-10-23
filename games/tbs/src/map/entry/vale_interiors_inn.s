@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ValeInteriorsInn_Initialize
	overlay_veneer ValeInteriorsInn_GetWarpDests
	overlay_veneer ValeInteriorsInn_GetWarps
	overlay_veneer ValeInteriorsInn_GetObjectEventTemplates
	overlay_veneer ValeInteriorsInn_GetEvents
	overlay_veneer ValeInteriorsInn_GetRegions
