@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer KolimaForestTree_Initialize
	overlay_veneer KolimaForestTree_GetEntrances
	overlay_veneer KolimaForestTree_GetExits
	overlay_veneer KolimaForestTree_GetPlacements
	overlay_veneer KolimaForestTree_GetEvents
	overlay_veneer KolimaForestTree_GetRegions
