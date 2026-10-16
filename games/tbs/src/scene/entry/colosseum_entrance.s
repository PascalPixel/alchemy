@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ColosseumEntrance_Initialize
	overlay_veneer ColosseumEntrance_GetEntrances
	overlay_veneer ColosseumEntrance_GetExits
	overlay_veneer ColosseumEntrance_GetPlacements
	overlay_veneer ColosseumEntrance_GetEvents
	overlay_veneer ColosseumEntrance_GetRegions
