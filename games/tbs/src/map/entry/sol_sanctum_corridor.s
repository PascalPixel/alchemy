@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer SolSanctumRooms_Initialize
	overlay_veneer Map_GetWarpDests
	overlay_veneer Map_GetWarps
	overlay_veneer Map_GetObjectEventTemplates
	overlay_veneer Map_GetEvents
	overlay_veneer Map_GetWarpRegions
