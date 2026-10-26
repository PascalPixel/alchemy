@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer VenusLighthouse_Initialize
	overlay_veneer VenusLighthouse_GetWarpDests
	overlay_veneer VenusLighthouse_GetWarps
	overlay_veneer VenusLighthouse_GetObjectEventTemplates
	overlay_veneer VenusLighthouse_GetEvents
	overlay_veneer VenusLighthouse_GetRegions
