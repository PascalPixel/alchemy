@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Scene_Initialize
	overlay_veneer LaliveroInteriors_GetEntrances
	overlay_veneer LaliveroInteriors_GetExits
	overlay_veneer LaliveroInteriors_GetPlacements
	overlay_veneer LaliveroInteriors_GetEvents
	overlay_veneer LaliveroInteriors_GetRegions
