@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer LogRoomsC_Initialize
	overlay_veneer LogRoomsC_GetWarpDests
	overlay_veneer LogRoomsC_GetWarps
	overlay_veneer LogRoomsC_GetObjectEventTemplates
	overlay_veneer LogRoomsC_GetEvents
	overlay_veneer LogRoomsC_GetRegions
