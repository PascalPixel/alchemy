@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer KalayTolbiDocks_Initialize
	overlay_veneer KalayTolbiDocks_GetEntrances
	overlay_veneer KalayTolbiDocks_GetExits
	overlay_veneer KalayTolbiDocks_GetPlacements
	overlay_veneer KalayTolbiDocks_GetEvents
	overlay_veneer KalayTolbiDocks_GetRegions
