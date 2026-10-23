@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer KolimaInteriors_Initialize
	overlay_veneer KolimaInteriors_GetWarpDests
	overlay_veneer KolimaInteriors_GetWarps
	overlay_veneer KolimaInteriors_GetObjectEventTemplates
	overlay_veneer KolimaInteriors_GetEvents
	overlay_veneer KolimaInteriors_GetRegions
