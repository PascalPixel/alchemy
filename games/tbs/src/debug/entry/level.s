@ C2: aligned 8-byte veneers: overlay_veneer emits ldr r4, bx r4 and a target word.
	.include "games/tbs/src/system/overlay.inc"
	overlay_veneer Map_RunLevelRoomSetup
	overlay_veneer ItemLevel_GetWarpDests
	overlay_veneer ItemLevel_GetWarps
	overlay_veneer ItemLevel_GetObjectEventTemplates
	overlay_veneer ItemLevel_GetEvents
	overlay_veneer MapData_ReturnZero
