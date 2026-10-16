@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer CrossboneIsle_Initialize
	overlay_veneer CrossboneIsle_GetEntrances
	overlay_veneer CrossboneIsle_GetExits
	overlay_veneer CrossboneIsle_GetPlacements
	overlay_veneer CrossboneIsle_GetEvents
	overlay_veneer CrossboneIsle_GetRegions
