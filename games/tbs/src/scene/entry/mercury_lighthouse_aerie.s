@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer MercuryLighthouseAerie_Initialize
	overlay_veneer MercuryLighthouseAerie_GetWarpDests
	overlay_veneer MercuryLighthouseAerie_GetWarps
	overlay_veneer MercuryLighthouseAerie_GetObjectEventTemplates
	overlay_veneer MercuryLighthouseAerie_GetEvents
	overlay_veneer MercuryLighthouseAerie_GetRegions
