@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Lalivero_Initialize
	overlay_veneer Lalivero_GetEntrances
	overlay_veneer Lalivero_GetExits
	overlay_veneer Lalivero_GetPlacements
	overlay_veneer Lalivero_GetEvents
	overlay_veneer Lalivero_GetRegions
