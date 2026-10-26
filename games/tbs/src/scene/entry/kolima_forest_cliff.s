@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer KolimaForestCliff_Initialize
	overlay_veneer KolimaForestCliff_GetWarpDests
	overlay_veneer KolimaForestCliff_GetWarps
	overlay_veneer KolimaForestCliff_GetObjectEventTemplates
	overlay_veneer KolimaForestCliff_GetEvents
	overlay_veneer KolimaForestCliff_GetRegions
