@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer CrossbonePillar_Initialize
	overlay_veneer CrossbonePillar_GetEntrances
	overlay_veneer CrossbonePillar_GetExits
	overlay_veneer CrossbonePillar_GetPlacements
	overlay_veneer CrossbonePillar_GetEvents
	overlay_veneer CrossbonePillar_GetRegions
