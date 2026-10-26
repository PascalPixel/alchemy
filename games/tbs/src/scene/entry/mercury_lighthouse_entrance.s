@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer MercuryLighthouseEntrance_Initialize
	overlay_veneer MercuryLighthouseEntrance_GetWarpDests
	overlay_veneer MercuryLighthouseEntrance_GetWarps
	overlay_veneer MercuryLighthouseEntrance_GetObjectEventTemplates
	overlay_veneer MercuryLighthouseEntrance_GetEvents
	overlay_veneer MercuryLighthouseEntrance_GetRegions
