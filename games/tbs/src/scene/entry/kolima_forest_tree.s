@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer KolimaForestTree_Initialize
	overlay_veneer KolimaForestTree_GetWarpDests
	overlay_veneer KolimaForestTree_GetWarps
	overlay_veneer KolimaForestTree_GetObjectEventTemplates
	overlay_veneer KolimaForestTree_GetEvents
	overlay_veneer KolimaForestTree_GetRegions
