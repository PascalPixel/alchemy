@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer GondowanCave_Initialize
	overlay_veneer GondowanCave_GetEntrances
	overlay_veneer GondowanCave_GetExits
	overlay_veneer GondowanCave_GetPlacements
	overlay_veneer GondowanCave_GetEvents
	overlay_veneer GondowanCave_GetRegions
