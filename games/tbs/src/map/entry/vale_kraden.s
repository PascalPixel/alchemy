@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ValeKraden_Initialize
	overlay_veneer ValeKraden_GetWarpDests
	overlay_veneer ValeKraden_GetWarps
	overlay_veneer ValeKraden_GetObjectEventTemplates
	overlay_veneer ValeKraden_GetEvents
	overlay_veneer ValeKraden_GetRegions
