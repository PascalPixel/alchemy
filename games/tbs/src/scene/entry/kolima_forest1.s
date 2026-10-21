@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer KolimaForest1_Initialize
	overlay_veneer KolimaForest1_GetEntrances
	overlay_veneer KolimaForest1_GetExits
	overlay_veneer KolimaForest1_GetPlacements
	overlay_veneer KolimaForest1_GetEvents
	overlay_veneer KolimaForest1_GetRegions
