@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ValeStorm_Initialize
	overlay_veneer ValeStorm_GetEntrances
	overlay_veneer ValeStorm_GetExits
	overlay_veneer ValeStorm_GetPlacements
	overlay_veneer ValeStorm_GetEvents
	overlay_veneer ValeStorm_GetRegions
