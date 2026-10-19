@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ColossoRiver_Initialize
	overlay_veneer ColossoRiver_GetEntrances
	overlay_veneer ColossoRiver_GetExits
	overlay_veneer ColossoRiver_GetPlacements
	overlay_veneer ColossoRiver_GetEvents
	overlay_veneer ColossoRiver_GetRegions
