@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer KolimaForestBridge_Initialize
	overlay_veneer KolimaForestBridge_GetEntrances
	overlay_veneer KolimaForestBridge_GetExits
	overlay_veneer KolimaForestBridge_GetPlacements
	overlay_veneer KolimaForestBridge_GetEvents
	overlay_veneer KolimaForestBridge_GetRegions
