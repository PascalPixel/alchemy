@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer McCoyPalace_Initialize
	overlay_veneer McCoyPalace_GetEntrances
	overlay_veneer McCoyPalace_GetExits
	overlay_veneer McCoyPalace_GetPlacements
	overlay_veneer McCoyPalace_GetEvents
	overlay_veneer McCoyPalace_GetRegions
