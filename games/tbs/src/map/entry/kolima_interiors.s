@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer KolimaInteriors_Initialize
	overlay_veneer KolimaInteriors_GetEntrances
	overlay_veneer KolimaInteriors_GetExits
	overlay_veneer KolimaInteriors_GetPlacements
	overlay_veneer KolimaInteriors_GetEvents
	overlay_veneer KolimaInteriors_GetRegions
