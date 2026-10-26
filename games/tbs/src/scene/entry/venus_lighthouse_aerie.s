@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer VenusLighthouseAerie_Initialize
	overlay_veneer VenusLighthouseAerie_GetWarpDests
	overlay_veneer VenusLighthouseAerie_GetWarps
	overlay_veneer VenusLighthouseAerie_GetObjectEventTemplates
	overlay_veneer VenusLighthouseAerie_GetEvents
	overlay_veneer VenusLighthouseAerie_GetRegions
