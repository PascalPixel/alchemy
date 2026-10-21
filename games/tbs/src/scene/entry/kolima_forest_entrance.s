@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer KolimaForestEntrance_Initialize
	overlay_veneer KolimaForestEntrance_GetEntrances
	overlay_veneer KolimaForestEntrance_GetExits
	overlay_veneer KolimaForestEntrance_GetPlacements
	overlay_veneer KolimaForestEntrance_GetEvents
	overlay_veneer KolimaForestEntrance_GetRegions
