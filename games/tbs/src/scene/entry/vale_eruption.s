@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ValeEruption_Initialize
	overlay_veneer ValeEruption_GetEntrances
	overlay_veneer ValeEruption_GetExits
	overlay_veneer ValeEruption_GetPlacements
	overlay_veneer ValeEruption_GetEvents
	overlay_veneer ValeEruption_GetRegions
