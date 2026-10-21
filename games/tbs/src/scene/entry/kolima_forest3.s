@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer KolimaForest3_Initialize
	overlay_veneer KolimaForest3_GetEntrances
	overlay_veneer KolimaForest3_GetExits
	overlay_veneer KolimaForest3_GetPlacements
	overlay_veneer KolimaForest3_GetEvents
	overlay_veneer KolimaForest3_GetRegions
