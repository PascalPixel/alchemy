@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ColosseumAzartMatch_Initialize
	overlay_veneer ColosseumAzartMatch_GetEntrances
	overlay_veneer ColosseumAzartMatch_GetExits
	overlay_veneer ColosseumAzartMatch_GetPlacements
	overlay_veneer ColosseumAzartMatch_GetEvents
	overlay_veneer ColosseumAzartMatch_GetRegions
