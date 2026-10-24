@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer TestRoom_Initialize
	overlay_veneer TestRoom_GetWarpDests
	overlay_veneer TestRoom_GetWarps
	overlay_veneer TestRoom_GetObjectEventTemplates
	overlay_veneer TestRoom_GetEvents
	overlay_veneer Map_GetWarpRegions
