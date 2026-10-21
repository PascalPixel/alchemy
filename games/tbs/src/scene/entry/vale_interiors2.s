@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ValeInteriors2_Initialize
	overlay_veneer ValeInteriors2_GetEntrances
	overlay_veneer ValeInteriors2_GetExits
	overlay_veneer ValeInteriors2_GetPlacements
	overlay_veneer ValeInteriors2_GetEvents
	overlay_veneer ValeInteriors2_GetRegions
