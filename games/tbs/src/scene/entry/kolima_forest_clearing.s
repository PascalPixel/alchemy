@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer KolimaForestClearing_Initialize
	overlay_veneer KolimaForestClearing_GetEntrances
	overlay_veneer KolimaForestClearing_GetExits
	overlay_veneer KolimaForestClearing_GetPlacements
	overlay_veneer KolimaForestClearing_GetEvents
	overlay_veneer KolimaForestClearing_GetRegions
