@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer KolimaForestRiver_Initialize
	overlay_veneer KolimaForestRiver_GetEntrances
	overlay_veneer KolimaForestRiver_GetExits
	overlay_veneer KolimaForestRiver_GetPlacements
	overlay_veneer KolimaForestRiver_GetEvents
	overlay_veneer KolimaForestRiver_GetRegions
