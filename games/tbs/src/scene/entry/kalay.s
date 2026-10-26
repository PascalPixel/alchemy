@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Kalay_Initialize
	overlay_veneer Kalay_GetWarpDests
	overlay_veneer Kalay_GetWarps
	overlay_veneer Kalay_GetObjectEventTemplates
	overlay_veneer Kalay_GetEvents
	overlay_veneer Kalay_GetRegions
