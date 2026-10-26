@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Bilibin_Initialize
	overlay_veneer Bilibin_GetWarpDests
	overlay_veneer Bilibin_GetWarps
	overlay_veneer Bilibin_GetObjectEventTemplates
	overlay_veneer Bilibin_GetEvents
	overlay_veneer Bilibin_GetRegions
