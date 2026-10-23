@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer MercuryLighthouse_Initialize
	overlay_veneer MercuryLighthouse_GetWarpDests
	overlay_veneer MercuryLighthouse_GetWarps
	overlay_veneer MercuryLighthouse_GetObjectEventTemplates
	overlay_veneer MercuryLighthouse_GetEvents
	overlay_veneer MercuryLighthouse_GetRegions
