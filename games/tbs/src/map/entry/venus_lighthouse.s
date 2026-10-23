@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer VenusLighthouse_Initialize
	overlay_veneer VenusLighthouse_GetEntrances
	overlay_veneer VenusLighthouse_GetExits
	overlay_veneer VenusLighthouse_GetPlacements
	overlay_veneer VenusLighthouse_GetEvents
	overlay_veneer VenusLighthouse_GetRegions
