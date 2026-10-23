@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer ColosseumEntrance_Initialize
	overlay_veneer ColosseumEntrance_GetWarpDests
	overlay_veneer ColosseumEntrance_GetWarps
	overlay_veneer ColosseumEntrance_GetObjectEventTemplates
	overlay_veneer ColosseumEntrance_GetEvents
	overlay_veneer ColosseumEntrance_GetRegions
