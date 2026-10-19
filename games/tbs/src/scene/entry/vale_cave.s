@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ValeCave_Initialize
	overlay_veneer ValeCave_GetEntrances
	overlay_veneer ValeCave_GetExits
	overlay_veneer ValeCave_GetPlacements
	overlay_veneer ValeCave_GetEvents
	overlay_veneer ValeCave_GetRegions
