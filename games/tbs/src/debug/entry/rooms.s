@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Map_RunEmpty
	overlay_veneer TestRooms_GetWarpDests
	overlay_veneer TestRooms_GetWarps
	overlay_veneer TestRooms_GetObjectEventTemplates
	overlay_veneer TestRooms_GetEvents
	overlay_veneer MapData_ReturnZero
