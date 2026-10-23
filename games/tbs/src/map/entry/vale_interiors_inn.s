@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ValeInteriorsInn_Initialize
	overlay_veneer ValeInteriorsInn_GetEntrances
	overlay_veneer ValeInteriorsInn_GetExits
	overlay_veneer ValeInteriorsInn_GetPlacements
	overlay_veneer ValeInteriorsInn_GetEvents
	overlay_veneer ValeInteriorsInn_GetRegions
