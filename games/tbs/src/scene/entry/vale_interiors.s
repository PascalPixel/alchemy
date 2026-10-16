@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ValeInteriors_Initialize
	overlay_veneer ValeInteriors_GetEntrances
	overlay_veneer ValeInteriors_SelectExits
	overlay_veneer ValeInteriors_SelectPlacements
	overlay_veneer ValeInteriors_SelectEvents
	overlay_veneer ValeInteriors_GetRegions
