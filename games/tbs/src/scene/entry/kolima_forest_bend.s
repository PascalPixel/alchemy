@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer KolimaForestBend_Initialize
	overlay_veneer KolimaForestBend_GetEntrances
	overlay_veneer KolimaForestBend_GetExits
	overlay_veneer KolimaForestBend_GetPlacements
	overlay_veneer KolimaForestBend_GetEvents
	overlay_veneer KolimaForestBend_GetRegions
