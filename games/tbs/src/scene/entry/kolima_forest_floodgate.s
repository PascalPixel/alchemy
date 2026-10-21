@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer KolimaForestFloodgate_Initialize
	overlay_veneer KolimaForestFloodgate_GetEntrances
	overlay_veneer KolimaForestFloodgate_GetExits
	overlay_veneer KolimaForestFloodgate_GetPlacements
	overlay_veneer KolimaForestFloodgate_GetEvents
	overlay_veneer KolimaForestFloodgate_GetRegions
