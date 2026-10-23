@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Xian_Initialize
	overlay_veneer Xian_GetWarpDests
	overlay_veneer Xian_GetWarps
	overlay_veneer Xian_GetObjectEventTemplates
	overlay_veneer Xian_GetEvents
	overlay_veneer Xian_GetRegions
