@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer TolbiSpring_Initialize
	overlay_veneer TolbiSpring_GetEntrances
	overlay_veneer TolbiSpring_GetExits
	overlay_veneer TolbiSpring_GetPlacements
	overlay_veneer TolbiSpring_GetEvents
	overlay_veneer TolbiSpring_GetRegions
