@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer LamakanDesert_Initialize
	overlay_veneer LamakanDesert_GetWarpDests
	overlay_veneer LamakanDesert_GetWarps
	overlay_veneer LamakanDesert_GetObjectEventTemplates
	overlay_veneer LamakanDesert_GetEvents
	overlay_veneer LamakanDesert_GetRegions
