@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Sanctum_Initialize
	overlay_veneer Sanctum_GetWarpDests
	overlay_veneer Sanctum_GetWarps
	overlay_veneer Sanctum_GetObjectEventTemplates
	overlay_veneer Sanctum_GetEvents
	overlay_veneer Sanctum_GetRegions
