@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer KolimaForestGlade_Initialize
	overlay_veneer KolimaForestGlade_GetWarpDests
	overlay_veneer KolimaForestGlade_GetWarps
	overlay_veneer KolimaForestGlade_GetObjectEventTemplates
	overlay_veneer KolimaForestGlade_GetEvents
	overlay_veneer KolimaForestGlade_GetRegions
